import unreal

# instances of unreal classes
editor_level_lib = unreal.EditorLevelLibrary()
editor_filter_lib = unreal.EditorFilterLibrary()

# get all level actors and filter by StaticMeshActors
actors = editor_level_lib.get_all_level_actors()
static_mesh_actors = editor_filter_lib.by_class(actors, unreal.StaticMeshActor)
deleted = 0

for actor in static_mesh_actors:
    actor_name = actor.get_fname()

    actor_mesh_comp = actor.static_mesh_component
    actor_mesh = actor_mesh_comp.static_mesh

    if actor_mesh is None:
        actor.destroy_actor()
        deleted += 1
        unreal.log("The mesh component of actor {} is invalid. The actor was deleted".format(actor_name))

unreal.log("Deleted {} actors".format(deleted))
