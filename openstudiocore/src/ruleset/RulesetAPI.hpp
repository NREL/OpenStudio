#ifndef RULESET_RULESETAPI_HPP
#define RULESET_RULESETAPI_HPP

#if _WIN32 || _MSC_VER

#ifdef openstudio_ruleset_EXPORTS
#define RULESET_API __declspec(dllexport)
#define RULESET_TEMPLATE_EXT 
#else
#define RULESET_API __declspec(dllimport)
#define RULESET_TEMPLATE_EXT extern
#endif
#else
#define RULESET_API
#endif

#endif
