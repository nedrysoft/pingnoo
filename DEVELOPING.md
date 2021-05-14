# Developing for Pingnoo

The modular architecture of pingnoo allows third parties to extend the features of the software.  Wherever it makes sense, the functionality is abstracted using pure virtual classes, known as interfaces.  These interfaces define a protocol that an extension to the application should implement to hook into the application.

In pingnoo, we refer to plugins as components.  The plugins are shared libraries (DLL's in windows speak) containing methods and metadata.  The component system loader searches for all available components. Using this metadata, the component system loader resolves any dependencies and then attempts to load each component in the correct order.

The actual application, located in src/app, is a minimal stub application. Its only purpose is to discover the plugins, figure out their dependencies, and initialise them in the correct order.

## Project layout

This guide aims to give a brief overview of the structure without going into too much detail. It intends to provide enough information for other developers to extend the capabilities of pingnoo.

## `/`

The project's root contains the main CMakeLists.txt and several python scripts used by the CI/CD system to create deployable images of pingnoo.

## `/src`

The src folder is where the actual application code resides, generally speaking, you should not create any files or folders at this level.

## `/src/app`

As previously mentioned, the main() entry point does very little other than find all plugins and resolve their dependencies and load order.  It is a wrapper around the ComponentSystem library which is the fundamental object in the software.

## `/src/common`

The common folder holds files that may be used project-wide, for example, a list of string identifiers for menus/commands.

## `/src/components`

All components in pingnoo should reside under the components folder.

## `/src/components/<component>`

You should structure your components in the same way that the included pingnoo components are structured, and this is to maintain compatibility and ensure that source code is consistent.

### `/src/component/<component>/SDK`

If your component exposes an SDK, you should provide an SDK folder containing the header files to allow other developers to use it.  The pingnoo "cmake meta language" has built-in support for components structured in this manner.

For example, to use the Core component from your component, you use the `pingnoo_use_component` command.

```
pingnoo_use_component(Core)
```

This command will automatically add the SDK path as a system include location, and it will also automatically link to the component. You do not have to do anything else; CMake will take care of everything for you.

### `/src/libs/<library>/include`

You should provide an include folder that contains the public header files that will allow other developers to use it.  The pingnoo "cmake meta language" has built-in support for libraries structured in this manner.

For example, to use the MyLibrary library, you can use the `pingnoo_use_library' command.

```
pingnoo_use_library(MyLibrary)
```

This command will automatically add the include path as a system include location, and it will also automatically link to the library. You do not have to do anything else; CMake will take care of everything for you.
