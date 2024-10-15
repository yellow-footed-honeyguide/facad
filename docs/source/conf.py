import os
import sys

project = 'Facad'
copyright = '2024, Sergey Veneckiy'
author = 'Sergey Veneckiy'
release = '2.19.0'

sys.path.insert(0, os.path.abspath('../..'))

extensions = [
    'sphinx.ext.autodoc',
    'sphinx.ext.intersphinx',
    'sphinx.ext.todo',
    'sphinx.ext.coverage',
    'sphinx.ext.mathjax',
    'sphinx.ext.viewcode',
    'breathe',
    'sphinx.ext.graphviz'
]

breathe_projects = {
    "facad": os.path.abspath('../../doxygen/xml')
}
breathe_default_project = "facad"

graphviz_output_format = 'svg'
graphviz_dot_args = ['-Gfontsize=10']

templates_path = ['_templates']
exclude_patterns = []

html_theme = 'sphinx_rtd_theme'
html_static_path = ['_static']

todo_include_todos = True

html_css_files = [
    'custom.css',
]
