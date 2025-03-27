# Configuration file for the Sphinx documentation builder.
#
# For the full list of built-in configuration values, see the documentation:
# https://www.sphinx-doc.org/en/master/usage/configuration.html

# -- Project information -----------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#project-information

import sphinx_rtd_theme

project = 't1x-server'
copyright = '2025, Wolf Pack'
author = 'Kazam'
release = 'v0.1.1-stable'

# -- General configuration ---------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#general-configuration

extensions = []

templates_path = ['_templates']
exclude_patterns = []



# -- Options for HTML output -------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#options-for-html-output

html_theme = "sphinx_rtd_theme"

html_static_path = ['_static']

html_sidebars = {'**': ['globaltoc.html']}
html_theme_options = {
    'collapse_navigation': False,  # Prevents collapsing of TOC subtrees
    'navigation_depth': 4,        # Ensures deeper levels are shown
    'sticky_navigation': True     # Keeps navigation visible while scrolling
}