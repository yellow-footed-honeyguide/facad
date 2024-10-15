Welcome to Facad's documentation!
=================================

.. toctree::
   :maxdepth: 2
   :caption: Contents:

Introduction
============

Facad is a modern, colorful directory listing tool for the command line.

.. image:: ../../assets/facad_2_18.jpg
   :alt: Facad in action

Features
--------

- Intuitive file type representation
- Sorted output with directories first
- Four-column layout for quick directory analysis
- Supports symlinks and executable files
- Sort extensions
- Unicode-aware formatting

Installation
============

Dependencies
------------

- Pure C implementation without any dependency

Arch Linux
----------

Just use the script from AUR:

.. code-block:: bash

   yay -S facad

Build from Source
-----------------

.. code-block:: bash

   git clone https://github.com/yellow-footed-honeyguide/facad.git
   cd facad
   mkdir build && cd build
   meson setup .. && ninja

Install
-------

For system-wide installation (may require elevated privileges like `sudo`):

.. code-block:: bash

   sudo ninja install

Usage
=====

- Run `facad` command for default grid file listing
- Run `facad -l` command for long listing output
- Run `facad -a` command for dir analytics output
- Run `facad -h` command for help
- Run `facad -v` command for version

Modules
=======

Main Program
------------

.. doxygenfile:: main.c
   :project: facad

.. image:: ../../doxygen/html/d0/d29/main_8c_a0ddf1224851353fc92bfbff6f499fa97_cgraph_org.svg
.. graphviz:
   :caption: Call graph for main function

Argument Parser
---------------

.. doxygenfile:: args_parser.c
   :project: facad

.. graphviz:: ../../doxygen/xml/args__parser_8c.xml
   :caption: Call graph for argument parsing functions

Directory Utilities
-------------------

.. doxygenfile:: dev_dir_utils.c
   :project: facad

.. graphviz:: ../../doxygen/xml/dev__dir__utils_8c.xml
   :caption: Call graph for directory utility functions

Directory Analytics
-------------------

.. doxygenfile:: dir_analytics.c
   :project: facad

.. graphviz:: ../../doxygen/xml/dir__analytics_8c.xml
   :caption: Call graph for directory analytics functions

Directory Configuration
-----------------------

.. doxygenfile:: dir_config.c
   :project: facad

.. graphviz:: ../../doxygen/xml/dir__config_8c.xml
   :caption: Call graph for directory configuration functions

Display Utilities
-----------------

.. doxygenfile:: display_utils.c
   :project: facad

.. graphviz:: ../../doxygen/xml/display__utils_8c.xml
   :caption: Call graph for display utility functions

Emoji Mappings
--------------

.. doxygenfile:: emoji_mappings.c
   :project: facad

.. graphviz:: ../../doxygen/xml/emoji__mappings_8c.xml
   :caption: Call graph for emoji mapping functions

Emoji Utilities
---------------

.. doxygenfile:: emoji_utils.c
   :project: facad

.. graphviz:: ../../doxygen/xml/emoji__utils_8c.xml
   :caption: Call graph for emoji utility functions

File Card
---------

.. doxygenfile:: file_card.c
   :project: facad

.. graphviz:: ../../doxygen/xml/file__card_8c.xml
   :caption: Call graph for file card functions

Git Integration
---------------

.. doxygenfile:: git_integration.c
   :project: facad

.. graphviz:: ../../doxygen/xml/git__integration_8c.xml
   :caption: Call graph for Git integration functions

Git Utilities
-------------

.. doxygenfile:: git_utils.c
   :project: facad

.. graphviz:: ../../doxygen/xml/git__utils_8c.xml
   :caption: Call graph for Git utility functions

Long Listing
------------

.. doxygenfile:: longlisting.c
   :project: facad

.. graphviz:: ../../doxygen/xml/longlisting_8c.xml
   :caption: Call graph for long listing functions

Contributing
============

Contributions are welcome! Please feel free to submit a Pull Request.

1. Fork the Project
2. Create your Feature Branch (`git checkout -b feature/AmazingFeature`)
3. Commit your Changes (`git commit -m 'Add some AmazingFeature'`)
4. Push to the Branch (`git push origin feature/AmazingFeature`)
5. Open a Pull Request

Author
======

**Sergey Veneckiy**

- Email: s.venetsky@gmail.com
- GitHub: `@yellow-footed-honeyguide <https://github.com/yellow-footed-honeyguide>`_

License
=======

Distributed under the MIT License. See `LICENSE` for more information.

Indices and tables
==================

* :ref:`genindex`
* :ref:`modindex`
* :ref:`search`
