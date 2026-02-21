package dev.blackoutburst.client

import dev.blackoutburst.bogel.window.Window
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
    while(Window.isOpen) {
        Window.clear()

        TilesManager.render()

        Window.update()
    }
}