# Nedrysoft Font Awesome for Qt

This library provides easy access to Font Awesome glyphs.  It embeds a copy of the FontAwesome fonts and makes them available to the application.

## Usage

To get the names of the fonts:

```c++
auto brandsFontName = Nedrysoft::FontAwesome::brandsName();
auto brandsFontName = Nedrysoft::FontAwesome::regularName();
auto brandsFontName = Nedrysoft::FontAwesome::solidName();

```

To get HTML text from a plaintext with FontAwesome tags:

```c++
QString html = Nedrysoft::FontAwesome::richText("Here is a robot [fas fa-robot]");
```

To get a QIcon from a glyph:

```C++
QIcon icon = Nedrysoft::FontAwesome::icon("[fas fa-robot]", 16, Qt::red);
```

## Requirements

* Qt 5
* CMake

## Building

To build the library, invoke CMake or open the CMakeLists.txt file in your preferred IDE.

Setting the following CMake variables allows the customisation of the build.

```
NEDRYSOFT_FONTAWESOME_LIBRARY_DIR=<dir>
```

Sets the output folder for the dynamic library; if omitted, you can find the binaries in the default location.

## Credits

None of this is possible without [FontAwesome](https://fontawesome.com).

# License

This project is open source and released under the GPLv3 licence.

Distributed as-is; no warranty is given.
