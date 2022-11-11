import os
import unreal


def duplicate_assets(num_copies):
    """
    Duplicates the selected Assets.

    :param num_copies: The amount of duplicates to create.
    """

    # instances of unreal classes
    editor_util = unreal.EditorUtilityLibrary()
    editor_asset_lib = unreal.EditorAssetLibrary()

    # get the selected assets
    selected_assets = editor_util.get_selected_assets()
    num_assets = len(selected_assets)

    total_num_copies = num_assets * num_copies
    text_label = "Duplicating Assets"
    running = True

    with unreal.ScopedSlowTask(total_num_copies, text_label) as slow_task:
        # display the dialog
        slow_task.make_dialog()

        for asset in selected_assets:
            asset_name = asset.get_fname()
            asset_path = editor_asset_lib.get_path_name_for_loaded_asset(asset)
            source_path = os.path.dirname(asset_path)

            for i in range(num_copies):
                # if user pressed the cancel button, stop
                if slow_task.should_cancel():
                    running = False
                    break

                new_name = "{}_{}".format(asset_name, i)
                destination_path = os.path.join(source_path, new_name)
                duplicate = editor_asset_lib.duplicate_asset(asset_path, destination_path)
                slow_task.enter_progress_frame(1)

                if duplicate is None:
                    unreal.log_warning("Duplicate from {} at {} already exists".format(source_path, destination_path))

            if not running:
                break

        unreal.log("{} asset/s duplicated {} times".format(num_assets, num_copies))


duplicate_assets(5)
