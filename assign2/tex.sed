# Escape \
s/\\/\\verb+\\+/

# Escape %
s/%/\\%/

# Escape ^
s/\^/\\\^/

# Replace --
s/--/-\\hspace\{.01cm\}-/

# Insert TEX document headers
1i\
\\documentstyle[11pt]{article}\
\\begin{document}

# Append TEX document closing tag and closing description tag
$a\
\\end{description}\
\\end{document}

# Center first line and wrap body in description tag 
1i\
\\begin{center} {\\bf
1a\
} \\end{center}\
\\begin{description}

# Handle headers
/^[A-Z][A-Z]*$/s/^/\\item\[/
/\\item\[/s/$/\] \\hfill \\\\/

# Terminate option lines
/^[ \t]*[+-]/s/$/ \\\\/
