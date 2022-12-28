import bpy

context = bpy.context
scene = context.scene
object = context.object

# loop through shape keys
for shape_key in bpy.data.shape_keys:

    # loop through frames from top to bottom
    for frame in range(scene.frame_end, -1, -1):
        
        # create keyframe if keyblock is valid
        if frame < len(shape_key.key_blocks):
            key_block = shape_key.key_blocks[frame]
            scene.frame_set(frame)
            shape_key.eval_time = frame * 10
            shape_key.keyframe_insert("eval_time")
