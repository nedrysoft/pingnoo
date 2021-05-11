# Developing for Pingnoo

Pingnoo uses a modular architecture, whereever it makes sense the functionaly has been abstracted using pure virtual classes, known as interfaces.  These interfaces define a protocol that an extension to the application should implement to hook into the applciation.

In pingnoo, we refer to plugins as components.  The plugins are shared libraries (DLL's in windows speak) which contain a number of required methods along with metadata.  The component system loader, searches for all available components and then using the metadata, resolves any dependencies and then attempts to load eaqch component in turn.

The actual application, located in src/app is very small stub application, it's only purpose is to discover the plugins, figure out their dependencies, and initialise them in the correct order.

## Project layout

The aim of this guide is to give a brief overview of the structure without going into too much detail, it is intended to give enough information to other developers to extend the capabilities of pingnoo.

## /

The root of the project contains the main CMakeLists.txt along with a number of python scripts which which are used by the CI/CD system to create deployable images of pingnoo.

## /src

The src folder is where the actual application code resides, generally speaking you should not create any files or folders at this level.

## /src/app

As previously mentioned, the main() entry point does very little other than find all plugins and resolve their dependencies and load order.  It is a wrapper around the ComponentSystem library which is the fundamental object in the software.

## /src/common

The common folder holes files that may be reused across the project, for example a list of string identifiers for menus/commands.

## /src/components

All components in pingnoo should reside under the components folder.

## /src/component/<component>

The layout of the component structure should follow the standard implemented in the existing components.  Some components may provide a SDK of their own, for example

/src/component/<component>/SDK
/src/libs
/src/libs/<library>
/src/libs/<library>/include

# installation folders