package dev.blackoutburst.client.tile

import dev.blackoutburst.bogel.maths.Vector2f
import dev.blackoutburst.bogel.utils.Color

data class Tile(
    val layer: Int,
    val texture: String,
    val position: Vector2f,
    val size: Vector2f,
    val color: Color,
)