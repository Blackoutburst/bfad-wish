package dev.blackoutburst.client.entity

import dev.blackoutburst.bogel.maths.Vector2f

abstract class Entity(
    val position: Vector2f = Vector2f(),
    val size: Vector2f = Vector2f()
) {
    abstract fun update()
    abstract fun render()
}