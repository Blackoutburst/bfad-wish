plugins {
    kotlin("jvm") version "2.3.0"
    id("com.gradleup.shadow") version "9.3.1"
    id("application")
}

application {
    mainClass.set("dev.blackoutburst.client.MainKt")
}

group = "dev.blackoutburst.client"
version = "1.0-SNAPSHOT"

repositories {
    mavenCentral()
}

dependencies {
    implementation(files("libs/Bogel-Reloaded.jar"))
}

kotlin {
    jvmToolchain(25)
}

tasks.test {
    useJUnitPlatform()
}