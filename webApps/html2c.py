def html_to_c_string(filename):
    with open(filename, 'r') as file:
        lines = file.readlines()

    c_string = 'const char psu_page[] = "HTTP/1.1 200 OK\\r\\nContent-Type: text/html\\r\\n\\r\\n'
    for line in lines:
        stripped_line = line.strip().replace('"', '\\"')
        c_string += stripped_line + '\\n'
    c_string += '";'

    return c_string

html_file = 'index.html'
c_string = html_to_c_string(html_file)
with open('html_page.c', 'w') as file:
    file.write(c_string)

print(f'HTML content from {html_file} has been converted and saved to html_page.c')
