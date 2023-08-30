import json

import pandas as pd

# need to do some funky business since the second header has a truncated number
# of commas
df = (
    pd.read_csv(
        "https://raw.githubusercontent.com/NREL/SAM/develop/deploy/libraries/Sandia%20Modules.csv",
        index_col=0,
        header=None,
    )
    .T.fillna("")
    .set_index(["Name", "Units", "[0]"])
    .T
)
df = df.apply(pd.to_numeric, errors="ignore")
df = df[df.select_dtypes("number").notnull().all(axis=1)]

df_cols = pd.DataFrame(df.columns.tolist(), columns=df.columns.names)
col_description = df_cols.set_index("[0]").to_dict(orient="index")

df.index = df.index.str.replace(r"[ ", "[", regex=False)
df.columns = df.columns.droplevel([0, 1])
data = df.to_dict(orient="index")

json_data = {"column_description": col_description, "data": data}

with open("Sandia_Modules.json", "w") as f:
    json.dump(json_data, f, indent=2)
