# Core Component

The core component is responsible for creating the main window and providing access to management classes such as the ribbon bar manager, status bar manager, editor manager etc.

## The SDK

The SDK folder contains the header files that provide the API for other components to use.  The header files use the std style and are system wide.

## Using the SDK

The pingnoo cmake file provides the `pingnoo_use_component` which handles the adding of the SDK include path along with the linker path and libraries.

```
pingnoo_use_component(Core)
```

Once this has been added to your CMakeLists.txt, the include files are available.

```c++
#include <IComponent>
#include <IEditorManager>
...
auto editorManager = IComponentManager::getInstance();

if (editorManager) {
    editorManager->openEditor(new MyEditorClass);
}
```
