package dev.blackoutburst.client.entity

import dev.blackoutburst.bogel.camera.Camera
import dev.blackoutburst.bogel.graphics.ColoredBox2D
import dev.blackoutburst.bogel.input.Keyboard
import dev.blackoutburst.bogel.maths.Vector2f
import dev.blackoutburst.bogel.utils.Color
import dev.blackoutburst.bogel.utils.Time
import dev.blackoutburst.bogel.window.Window
import dev.blackoutburst.client.tile.TilesManager
import org.lwjgl.glfw.GLFW.GLFW_KEY_W
import org.lwjgl.glfw.GLFW.GLFW_KEY_A
import org.lwjgl.glfw.GLFW.GLFW_KEY_S
import org.lwjgl.glfw.GLFW.GLFW_KEY_D
import kotlin.math.sqrt


class EntityPlayer(
    var position: Vector2f = Vector2f(),
    var size: Vector2f = Vector2f(32f),
    var radius: Float = 16f,
) {
    companion object {
        private const val COLLISION_LAYER = 1

    }

    val playerSprite = ColoredBox2D(position.x, position.y, size.x, size.y, Color.WHITE, 100f)

    private val speed = 0.25f
    private val velocity = Vector2f()

    private fun handleInput() {
        velocity.set(0f, 0f)

        if (Keyboard.isKeyDown(GLFW_KEY_W)) velocity.y += 1f
        if (Keyboard.isKeyDown(GLFW_KEY_S)) velocity.y -= 1f
        if (Keyboard.isKeyDown(GLFW_KEY_A)) velocity.x -= 1f
        if (Keyboard.isKeyDown(GLFW_KEY_D)) velocity.x += 1f

        if (velocity.length() > 0f) velocity.normalize().mul(speed)
    }

    private fun moveAndCollide(dt: Float) {
        position.x += velocity.x * dt
        resolveCollisions()

        position.y += velocity.y * dt
        resolveCollisions()
    }

    private fun resolveCollisions() {
        val collisionLayer = TilesManager.layers
            .firstOrNull { it.order == COLLISION_LAYER } ?: return

        for (tile in collisionLayer.tiles) {
            val (penetration, normal) = circleVsAabb(
                circleCenter = position,
                circleRadius = radius,
                rectPos = tile.position,
                rectSize = tile.size
            ) ?: continue

            position.x += normal.x * penetration
            position.y += normal.y * penetration

            val dot = velocity.x * normal.x + velocity.y * normal.y
            if (dot < 0f) {
                velocity.x -= dot * normal.x
                velocity.y -= dot * normal.y
            }
        }
    }

    private fun circleVsAabb(
        circleCenter: Vector2f,
        circleRadius: Float,
        rectPos: Vector2f,
        rectSize: Vector2f
    ): Pair<Float, Vector2f>? {

        val rectMinX = rectPos.x
        val rectMinY = rectPos.y
        val rectMaxX = rectPos.x + rectSize.x
        val rectMaxY = rectPos.y + rectSize.y

        val closestX = circleCenter.x.coerceIn(rectMinX, rectMaxX)
        val closestY = circleCenter.y.coerceIn(rectMinY, rectMaxY)

        val dx = circleCenter.x - closestX
        val dy = circleCenter.y - closestY
        val distSq = dx * dx + dy * dy

        if (distSq > 0f) {
            if (distSq >= circleRadius * circleRadius) return null

            val dist = sqrt(distSq)
            val normal = Vector2f(dx / dist, dy / dist)
            val penetration = circleRadius - dist
            return Pair(penetration, normal)
        }

        val overlapLeft   = circleCenter.x - rectMinX
        val overlapRight  = rectMaxX - circleCenter.x
        val overlapTop    = circleCenter.y - rectMinY
        val overlapBottom = rectMaxY - circleCenter.y

        val minOverlap = minOf(overlapLeft, overlapRight, overlapTop, overlapBottom)

        val normal = when (minOverlap) {
            overlapLeft   -> Vector2f(-1f,  0f)
            overlapRight  -> Vector2f( 1f,  0f)
            overlapTop    -> Vector2f( 0f, -1f)
            else          -> Vector2f( 0f,  1f)
        }

        val penetration = circleRadius + minOverlap
        return Pair(penetration, normal)
    }

    fun update() {
        val dt = Time.delta.toFloat()
        handleInput()
        moveAndCollide(dt)

        playerSprite.x = position.x - radius
        playerSprite.y = position.y - radius

        Camera.position.x = -position.x + (Window.width / 2f)
        Camera.position.y = -position.y + (Window.height / 2f)
        Camera.view.setIdentity()
            .translate(Camera.position.x, Camera.position.y, Camera.position.z)
    }

    fun render() {
        playerSprite.render()
    }

}