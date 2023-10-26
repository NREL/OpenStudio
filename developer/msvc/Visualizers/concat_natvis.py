"""
This small snippet will open all the *.natvis files and concat them into a
single one to be able to use them in Visual Studio Code (for Mac for eg)
vscode-cpptools expects only one visualizerFile currently
"""

import glob as gb
import xml.etree.ElementTree as ET

OUT_FILENAME = "all_concat.natvis"


def concat_all_natvis_files():
    """
    Main function
    """
    natvis_files = gb.glob("*.natvis")
    if OUT_FILENAME in natvis_files:
        natvis_files.remove(OUT_FILENAME)

    # In order to avoid the ns0 prefix the default namespace should be set
    schema = "http://schemas.microsoft.com/vstudio/debugger/natvis/2010"
    # before reading the XML data.
    ET.register_namespace("", schema)

    # Open the file first found
    print("Opening first: {}".format(natvis_files[0]))
    tree = ET.parse(natvis_files[0])
    root = tree.getroot()

    ori_n_child = len(root)
    total_n_child = ori_n_child

    # Loop on the rest
    for fname in natvis_files[1:]:
        tree2 = ET.parse(fname)
        root2 = tree2.getroot()
        n_child = len(root2)
        print("Parsing '{}', {} children".format(fname, n_child))
        total_n_child += n_child
        # Append each child or root2 to the first root
        for child in root2.iter():
            root.append(child)

    final_n_child = len(root)
    if final_n_child == total_n_child:
        print("\nOK: Started with {}, Ended with {} children as " "expected".format(ori_n_child, final_n_child))
    else:
        print(
            "\nProblem: Started with {}, Ended with {} children, "
            "expected {}".format(ori_n_child, final_n_child, total_n_child)
        )

    print("\nSaving to {}".format(OUT_FILENAME))
    tree.write(OUT_FILENAME, encoding="utf-8", xml_declaration=True)


# If run from a terminal
if __name__ == "__main__":
    concat_all_natvis_files()
