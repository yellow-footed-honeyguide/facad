# facad

A modern, colorful directory listing tool for the command line.

![facad in action](assets/demo_facad.jpg)
![facad in action](assets/dir_analitics.jpg)

## Features

- 🎨 Emoji-based file and directory icons
- 📁 Sorted output with directories first
- 🔗 Supports symlinks and executable files
- 🧩 Compact grid display
- 📏 Customizable column width
- 🌐 Unicode-aware formatting

### Building from Source

1. Clone the repository:

```
git clone https://github.com/yellow-footed-honeyguide/facad.git
cd facad
mkdir build && cd build
meson setup ..
ninja
```

For system-wide installation:

```
sudo ninja install
```

## Usage

Run `facad` command for default grid file listing
Run `facad -l` command for long listing output
Run `facad -i` command for dir analytics output


## Contributing

Contributions are welcome! Please feel free to submit a Pull Request.

- Fork the Project
- Create your Feature Branch ( `git checkout -b feature/AmazingFeature` )
- Commit your Changes ( `git commit -m 'Add some AmazingFeature'` )
- Push to the Branch ( `git push origin feature/AmazingFeature` )
- Open a Pull Request

## Author
**Sergey Veneckiy**
- Email: s.venetsky@gmail.com
- GitHub: [@yellow-footed-honeyguide](https://github.com/yellow-footed-honeyguide)


## License
Distributed under the MIT License. See LICENSE for more information.
