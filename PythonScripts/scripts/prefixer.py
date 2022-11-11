import json
import unreal

"""
Adding prefixes to assets.

See prefix mapping in prefix_mapping.json
"""

#prefix_mapping_filename = "...configs\\prefix_mapping.json"

# instances of unreal classes
system_lib = unreal.SystemLibrary()
editor_util = unreal.EditorUtilityLibrary()

# prefix mapping
with open(prefix_mapping_filename, "r") as json_file:
    prefix_mapping = json.loads(json_file.read())

# get the selected assets
selected_assets = editor_util.get_selected_assets()
num_assets = len(selected_assets)
prefixed = 0

for asset in selected_assets:
    asset_name = system_lib.get_object_name(asset)
    asset_class = asset.get_class()
    class_name = system_lib.get_class_display_name(asset_class)

    class_prefix = prefix_mapping.get(class_name, None)

    if class_prefix is None:
        unreal.log_warning("No mapping for asset {} of type {}".format(asset_name, class_name))
        continue

    if not asset_name.startswith(class_prefix):
        new_name = class_prefix + asset_name
        editor_util.rename_asset(asset, new_name)
        prefixed += 1
        unreal.log("Prefixed {} of type {} with {}".format(asset_name, class_name, class_prefix))
    else:
        unreal.log("Asset {} of type {} is already prefixed with {}".format(asset_name, class_name, class_prefix))

unreal.log("Prefixed {} of {} assets".format(prefixed, num_assets))
