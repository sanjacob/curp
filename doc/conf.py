# Configuration file for the Sphinx documentation builder.
#
# For the full list of built-in configuration values, see the documentation:
# https://www.sphinx-doc.org/en/master/usage/configuration.html

# -- Project information -----------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#project-information

project = 'CURP'
copyright = '2025, Jacob Sánchez Pérez'
author = 'Jacob Sánchez Pérez'
release = '0.1.0'

# -- General configuration ---------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#general-configuration

extensions = ['sphinx.ext.mathjax']

templates_path = ['_templates']
exclude_patterns = ['_build', 'Thumbs.db', '.DS_Store']

language = 'es'

# -- Options for HTML output -------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#options-for-html-output

html_theme = 'grafica'
#html_logo = 'https://framework-gb.cdn.gob.mx/gobmx/img/logo_blanco.svg'
html_static_path = ['_static']
html_theme_options = {
    'description': 'Validación y análisis de la CURP Mexicana.',
    'github_user': 'sanjacob',
    'github_repo': 'curpsuite',
    'author_url': 'https://github.com/sanjacob/',
    'twitter_user': 'TheRoaringKitty',
    'instagram_user': 'TheRoaringKitty',
    'youtube_user': 'TheRoaringKitty',
    'extra_nav_links': {
        'Google': 'https://google.com'
    }
}

html_use_opensearch = 'https://curp.org'
