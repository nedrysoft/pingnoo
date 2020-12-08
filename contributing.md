
# Contributing to the project

The following document contains a set of guidelines for contributing to the project.  Any pull-requests should be compliant with this guide.  Code style is often fluent, open a GitHub issue for suggestions or improvements.

## C++ Version

Code should target the C++17 standard wherever possible. (development tools permitting)

## Header Files

Header files should always start with a copyright block.

```c++
/*
 * Copyright (C) 2020 Adrian Carpenter
 *
 * This file is part of Pingnoo
 *
 * Created by Adrian Carpenter on 05/12/2020.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
```

## Includes

Include files are logically grouped according to their scope and sorted alphabetically for clarity.

### Source file

1. The first header should be the corresponding header to the source file.
2. *Empty Line*
3. Local includes (sorted alphabetically)
4. *Empty Line*
5. System includes (sorted alphabetically)

### Header file

1. Local includes (sorted alphabetically)
2. *Empty Line*
3. System includes (sorted alphabetically)
4. *Empty Line*
5. Objective C++ imports (sorted alphabetically)

### Exceptions

If a file requires including at a specific location (i.e. either the first or last), then it should be accompanied a comment explaining why the file is out of sequence.

# C++

## Include Guards

All include files should have an include guard to prevent the file from being included more than once.

```c++
#ifndef NEDRYSOFT_RIBBONPUSHBUTTON_H
#define NEDRYSOFT_RIBBONPUSHBUTTON_H
...
#endif // NEDRYSOFT_RIBBONPUSHBUTTON_H
```

The format of an include guard should be as follows:

```c++
[ORGANISATION]_[FILENAME]_H
```

## Namespaces

All code must exist in an appropriate namespace to prevent clashes with third party code, when using an item from a namespace then the fully qualified name should be used, and "using <namespace>" should not be used, using the full decorated namespace makes it much easier to locate the items origin.

* Use C++17 nested namespaces.
* Braces should be on the same line as the namespace definition.
* Items inside a namespace indent 1 level

*Good*

```c++
namespace Nedrysoft::Utils {
    ...
}
```

*Bad*

```c++
namespace Nedrysoft {
    namespace Utils {
        ...
    }
}
```
```c++
namespace Nedrysoft
{
    ...
}
```

### Constants

For constant values, use ```constexpr``` wherever possible rather than ```#define```.  Constants should be limited to a namespace or the class scope and should not be global.

*Good*

```c++
constexpr auto RibbonPushButtonDefaultIconWidth = 32;
constexpr auto RibbonPushButtonDefaultIconHeight = 48;
constexpr auto RibbonPushButtonDefaultFontSize = 10;
```

*Bad*

```c++
#define RibbonPushButtonDefaultIconWidth 32
#define RibbonPushButtonDefaultIconHeight = 48
#define RibbonPushButtonDefaultFontSize = 10;
```

### Line Breaks

Line breaks are permitted when they improve clarity, use a double indent on the following lines.

```c++
QString filename =
        fore(AnsiColour::WHITE)+
        "\""+
        fore(0xb0,0x85, 0xbe)+
        fileInfo.fileName()+
        fore(AnsiColour::WHITE)+
        "\""+
        fore(AnsiColour::YELLOW);
```

### Line Wrapping

Limit lines to 120 characters whenever possible, a blank line separates the parameters from the next block.  There should be one parameter per line starting from the line following the open brace.

In the case of functions with initialisers then indent the initialisers one additional level.

For function calls, parameters appear on separate lines and are indented two levels, and a space character should precede the closing brace.

*Good*

```c++
QString Nedrysoft::RouteAnalyser::CPAxisTickerMS::getTickLabel(
        double tick,
        const QLocale &locale,
        QChar formatChar,
        int precision) {

...
}
```

```c++
QString Nedrysoft::RouteAnalyser::CPAxisTickerMS::getTickLabel(
        double tick,
        const QLocale &locale,
        QChar formatChar,
        int precision) : 
            
            m_initExample(0),
            m_anotherExmaple(1) {

...
}
```

### Casting

C++ style casts are required, do not use C style casts.

*Good*

```c++
auto asFloat = static_cast<float>(exampleInteger);
```

*Bad*
```c++
auto asFloat = (float) exampleInteger;
```

### Comments

Use comments where appropriate, code should use descriptive names which alleviates the need for large blocks of comments.  Use Comments to describe operations that may not be obvious.

### Comment Format

Comments use the JavaDoc style for consistency.

*Good*

```c++
/**
 * @brief       Reimplements: QObject::eventFilter(QObject *watched, QEvent *event).
 *
 * @param[in]   watched the object that caused the event.
 * @param[in]   event the event information.
 *
 * @returns     true if event was handled, otherwise false.
 */
bool eventFilter(QObject *watched, QEvent *event) override;
```

* The comment must come directly before the method and there should be no gap between the comment and the function declaration.
* A new line must exist between each section of the comment; group parameters without newlines.
* The ```@returns``` tag should be used instead of ```@return```.
* Parameters should include the direction of data.

### Class declarations

The scope modifier should be indented one level, and functions and variables also should be indented a further level.

All class definitions should adhere to the following style:

*Good*

```c++
class RibbonTabBar :
    public QTabBar,
    public MyInterface {
        ...
    }
};
```
```c++
class RibbonTabBar {
    ...
};
```

Bad

```c++
class RibbonTabBar 
{
    ...
};
```
```c++
class RibbonTabBar {
public:
    ...
};
```
```c++
class RibbonTabBar 
{
public:
    ...
};
```

### Class layout

The class definition groups items by their scope and type, enums have their own scope, and function definitions have their own scope and so on.

```c++
class RibbonTabBar :
    public QTabBar,
    public MyInterface {
        private:¹
            Q_OBJECT

            Q_INTERFACES(MyInterfaces)
        
        public:²
            enum RibbonType {
                Standard,
                Fresh
            };
            
        public:³
            /**
             * @brief       Constructs a new RibbonTabBar instance which is a child of the parent.
             *
             * @param[in]   parent the owner widget.
             */
            explicit RibbonTabBar(QWidget *parent = nullptr);
    
        protected:⁴
            /**
             * @brief       Reimplements: QWidget::paintEvent(QPaintEvent *event).
             *
             * @param[in]   event contains information for painting this widget
             */
            void paintEvent(QPaintEvent *event) override;

        private:⁵
            /**
             * @brief       Updates the widgets stylesheet when the operating system theme is changed.
             *
             * @param[in]   isDarkMode true if dark mode; otherwise false.
             */
            void updateStyleSheet(bool isDarkMode);
    
        private:⁶
            QFont m_selectedFont;                                   //! the font to use on the selected tab
    }
}
```

1. This section should implicitly set the scope to private and should ***only*** include Qt macros.  Group macros logically with a new line separating each group.
2. This section (there may be more than one depending on scope) should define any ```enums``` or ```constexpr```.
3. This section includes constructors, destructors, and any public methods that the class exposes.
4. This section contains any overridden superclass functions.
5. This section (there may be more than one depending on scope) contains internal functions.
6. This section contains any member variables.

### Case

Class names should start (wherever possible) with a capital letter, and then capitalise the start of each other word.

Functions should use camelCase.

*Good*

```c++
class MyClass {
    public:
        void doSomething();
};
```

*Bad*
```c++
class MyClass {
    public:
        void DoSomething();
};
```
```c++
class myclass {
    public:
        void DoSomething();
};
```

### Member variables

Prefix member variables with ```m_```. and use camelCase.

```c++
class MyClass {
    public:
        ...
        
    private:
        int m_totalCount;
};
```

## Source Files

Source files generally follow the same style as set out for include files; all code should adhere to these rules.

### Function Definition

Function names should be descriptive and whenever possible, and not truncated.  Using descriptive naming makes the code easier to follow and more precise and negates the need for additional comments.

Each function definition must conform to the following code standard.

1. Member variables should use initialiser lists instead of being assigned.
2. The colon for the initialiser list must appear on the same line as the function definition.
3. Each initialiser is indented by two levels.
3. One initialiser per line.
4. The final initialiser contains the opening brace for the function.
5. A blank line should always follow the opening brace for constructors that have initialisers regardless of whether or not any statements follow.  For constructors, without initialisers, the blank line should be omitted.
6. Splitting parameters onto separate lines to increase readability is allowed.  A newline follows the opening brace, and there should be one line per parameter.  The parameters are indented two levels, and the closing brace and curly brace follows the final parameter, a blank line should also follow.

*Good*

```c++
RibbonDropButtonPlugin::RibbonDropButtonPlugin(QObject *parent) :
        QObject(parent),
        m_initialized(false) {
    
    ...
}
```
```c++
Nedrysoft::Image::Image() :
        m_data(nullptr),
        m_width(0),
        m_height(0),
        m_stride(0),
        m_imageId(0),
        m_isValid(false),
        m_length(0) {
    <blank line>
}
```
```c++
MyClass::MyFunction(
        QObject *parent, 
        Nedrysoft::ThisIsALongNamespaceEncapsulatedParameter *param1,
        Nedrysoft::ThisIsALongNamespaceEncapsulatedParameter *param2,
        Nedrysoft::ThisIsALongNamespaceEncapsulatedParameter *param1,
        Nedrysoft::ThisIsALongNamespaceEncapsulatedParameter *param2) {

    ...
}
```
```c++
RibbonDropButtonPlugin::RibbonDropButtonPlugin(QObject *parent) :
        QObject(parent),
        m_initialized(false) {
    
    doSomething();
    ...
}
```
```c++
RibbonDropButtonPlugin::RibbonDropButtonPlugin(QObject *parent) {
    ...
}
```
```c++
RibbonDropButtonPlugin::RibbonDropButtonPlugin(QObject *parent) {
    doSomething();
    ...
}
```

*Bad*

```c++
RibbonDropButtonPlugin::RibbonDropButtonPlugin(QObject *parent) : QObject(parent), m_initialized(false) {
    ...
}
```
```c++
RibbonDropButtonPlugin::RibbonDropButtonPlugin(QObject *parent) : QObject(parent), m_initialized(false)
{
    ...
}
```
```c++
RibbonDropButtonPlugin::RibbonDropButtonPlugin(QObject *parent) :
    QObject(parent),
    m_initialized(false) {
    
    ...
}
```
```c++
RibbonDropButtonPlugin::RibbonDropButtonPlugin(QObject *parent) :
        QObject(parent),
        m_initialized(false) {
    doSomething();
    ...
}
```
```c++
Nedrysoft::Image::Image() :
        m_data(nullptr),
        m_width(0),
        m_height(0),
        m_stride(0),
        m_imageId(0),
        m_isValid(false),
        m_length(0) {
}
```
```c++
MyClass::MyFunction(QObject *parent, Nedrysoft::ThisIsALongNamespaceEncapsulatedParameter *param1, Nedrysoft::ThisIsALongNamespaceEncapsulatedParameter *param2, Nedrysoft::ThisIsALongNamespaceEncapsulatedParameter *param1, Nedrysoft::ThisIsALongNamespaceEncapsulatedParameter *param2) {
    ...
}
```
```c++
MyClass::MyFunction(
        QObject *parent, 
        Nedrysoft::ThisIsALongNamespaceEncapsulatedParameter *param1,
        Nedrysoft::ThisIsALongNamespaceEncapsulatedParameter *param2,
        Nedrysoft::ThisIsALongNamespaceEncapsulatedParameter *param1,
        Nedrysoft::ThisIsALongNamespaceEncapsulatedParameter *param2) {
    doSomething();
    ...
}
```
### Return values

Omit braces for return values.

*Good*

```c++
bool MyTest::isFinished() {
    return m_isFinished;
}
```

*Bad*

```c++
bool MyTest::isFinished() {
    return(m_isFinished);
}
```

### Program flow

The program flow style is an extension of the rules described above.

#### if conditional

* Braces should always be used, even in the case of a single-line statement.
* Braces appear on the same line as the terminating conditional statement
* Multiple conditions may be split across multiple lines if it improves clarity.
* Each conditional must be surrounded by braces to avoid ambiguity.

*Good*

```c++
if (m_value>10) {
    ...
}
```
```c++
if (m_value>10) {
    ...
} else {
    ...
}
```
```c++
if (m_value>10) {
    ...
} else if (m_value>5) {
    ...
}
```
```c++
if ((m_fileLoaded) && 
    (m_fileSize<1024)) {
    ...
}
```

*Bad*

```c++
if (m_value>10)
{
    ...
}
```
```c++
if (m_value>10)
    ...
```
```c++
if (m_value>10)
    ...
else
    ...
```

#### for loops

* Wherever possible use range-based loops or iterators.
* Use descriptive names for indices or iterators

*Good*

```c++
for (auto fileEntry : files) {
    ...
}
```
```c++
for (auto currentIndex=0; currentIndex<totalValues; currentIndex++) {
    ...
}
```

#### while/do loops

* Braces appear on the same line as the loop.

*Good*

```c++
while(isRunning) {
    ...
}
```
```c++
do {
    ...
} while(isRunning);
```

*Bad*

```c++
while(isRunning)
{
    ... 
}
```
```c++
do
{
    ...
} while(isRunning);
```

#### Switch statements

* Cases are indented by one level.
* Multiple cases may be combined.
* Braces must always be used.
* The opening case brace is on the same line as the final case statement.

*Good*

```c++
switch(m_currentMode) {
    case Loading: {
        ...
        break;
    }
    
    case Saving: {
        ...
        break;
    }
    
    case Reading:
    case Writing: {
        ....
        break;
    }
    
    default: {
        ...
        break;
    }
}
```

*Bad*

```c++
switch(m_currentMode) {
case Loading: {
    ...
    break;
}
}
```
```c++
switch(m_currentMode) {
    case Loading:
        ...
        break;
}
```

#### Lambda functions

* Use of lambda expressions where possible, this is particularly true when dealing with asynchronous operations where using a lambda expression will result in cleaner and more readable code.
* std::function should be used as it allows both lambda and callback style operations.

```c++
void MyClass::downloadFile(DownloadInfo downloadInfo, std::function<void(DownloadStatus status)> statusFunction) {
    statusFunction(DownloadStatus(InitiatingDownload));
    
    startDownload(downloadInfo, statusFunction);
}
```

#### Singletons

* Singletons may be used where appropriate, for example, the MainWindow of the application may be accessed through a singleton instance which provides access to the object across the project.
* Singletons must use the C++11 pattern shown below as this inherently thread-safe.

```c++
MyClass::getInstance() {
    static MyClass *instance = new MyClass;
    
    return instance;
}
```


### Typing

Use ```auto```, it produces cleaner looking code.  Use primitive types (such as int, float, long) if auto does not make sense or results in ambiguity.

*Good*

```c+++
auto settingsDialog = SettingsDialog();
```

*Bad*

```c+++
QSettingsDialog settingsDialog;
```

### Qt Specific

* Qt keyword definitions should be disabled as they may conflict with third party code.  
* Use ```Q_EMIT Q_SIGNAL Q_SLOT``` macros.

```c++
class MyObject : 
        public QObject {
        
    private:
        Q_OBJECT
        
    public:
        Q_SLOT valueChanged(int newValue);
        
        Q_SIGNAL finished();
};
```

### Variables

Variable names should be descriptive; the more descriptive the name, the easier the code is to understand.  Variables should be named using camelCase.

*Good*

```c++
for (int currentCharacter=0;currentCharacter<stringLength;currentCharacter++) {
    
}
```

*Bad*

```c++
for (int i=0;i<j;i++) {
    ...
}
```

# Commit Messages

All commits to the repository must follow the conventional commit style.

The following tags are in use:

* "repo:" a change to the repo, includes operations such as editing README.md or restructuring.
* "wip:" a work in progress message.
* "refactor:" refactoring of the code.
* "chore:" general housekeeping task such as removing dead files from the repo.
* "fix:" a fix of a specific issue should typically be link reference a GitHub issue.
* "feat:" work on a specific feature.

Open a GitHub issue for suggestions of other tag types. 

