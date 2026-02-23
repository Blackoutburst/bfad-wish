package dev.blackoutburst.client

import dev.blackoutburst.bogel.camera.Camera
import dev.blackoutburst.bogel.graphics.Text
import dev.blackoutburst.bogel.graphics.Texture
import dev.blackoutburst.bogel.utils.Color
import dev.blackoutburst.bogel.window.Window
import dev.blackoutburst.client.entity.EntityPlayer
import dev.blackoutburst.client.map.MapLoader
import dev.blackoutburst.client.tile.TilesManager
import dev.blackoutburst.client.utils.ResourceFile
import org.lwjgl.opengl.GL11.GL_BLEND
import org.lwjgl.opengl.GL11.GL_ONE_MINUS_SRC_ALPHA
import org.lwjgl.opengl.GL11.GL_SRC_ALPHA
import org.lwjgl.opengl.GL11.GL_TEXTURE_2D
import org.lwjgl.opengl.GL11.glBlendFunc
import org.lwjgl.opengl.GL11.glEnable
import org.lwjgl.opengl.GL30.GL_TEXTURE_2D_ARRAY

object Main {
    val tileTextureFolder = ResourceFile("/textures/tiles/").url.readText().split("\n")
        .filter { it.isNotBlank() && it.endsWith(".png", ignoreCase = true) }
        .map { it.trim() }
}

fun main() {
    Window.setTitle("Born From a Dream (Wish Edition)").setVsync(false)

    glEnable(GL_BLEND)
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA)
    glEnable(GL_TEXTURE_2D)
    glEnable(GL_TEXTURE_2D_ARRAY)

    MapLoader.loadMap("map1.2de")

    update()
}

fun update() {
    val player = EntityPlayer()
    val position = Text(0f, 0f, 32f, "")
    val position2 = Text(0f, 0f, 32f, "")

    while(Window.isOpen) {
        Window.clear()


        player.update()

        TilesManager.render()

        player.render()
        position.x = -Camera.position.x
        position.y = -Camera.position.y + 1.5f
        position.text = "X: ${String.format("%.2f", player.position.x)}  Y: ${String.format("%.2f", player.position.y)}"

        position2.x = -Camera.position.x + 1.5f
        position2.y = -Camera.position.y
        position2.text = "&7X: ${String.format("%.2f", player.position.x)}  Y: ${String.format("%.2f", player.position.y)}"

        position2.render()
        position.render()

        Window.update()
    }
}