import unreal

# instances of unreal classes
system_lib = unreal.SystemLibrary()
editor_util = unreal.EditorUtilityLibrary()

# get the selected assets
selected_assets = editor_util.get_selected_assets()

for asset in selected_assets:
    asset_name = asset.get_fname()
    asset_class = asset.get_class()
    class_name = system_lib.get_class_display_name(asset_class)

    unreal.log("{} is {}".format(asset_name, asset_class))
