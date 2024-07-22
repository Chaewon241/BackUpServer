import argparse
import xml.etree.ElementTree as ET
import jinja2

def parse_xml(xml_path):
    tree = ET.parse(xml_path)
    root = tree.getroot()

    tables = []

    for table in root.findall('Table'):
        table_name = table.get('name')
        columns = []
        for column in table.findall('Column'):
            column_info = {
                'name': column.get('name'),
                'type': column.get('type'),
                'notnull': column.get('notnull', 'false') == 'true'
            }
            if column_info['type'].startswith('nvarchar'):
                column_info['type'] = 'std::wstring'
            elif column_info['type'].startswith('int'):
                column_info['type'] = 'int'
            elif column_info['type'].startswith('varbinary'):
                column_info['type'] = 'std::vector<char>'
            else:
                column_info['type'] = 'std::string'  # Default fallback
            columns.append(column_info)
        tables.append({
            'name': table_name,
            'columns': columns
        })

    return tables

def generate_cpp_header(tables, template_path, output_path):
    file_loader = jinja2.FileSystemLoader('templates')
    env = jinja2.Environment(loader=file_loader)
    template = env.get_template(template_path)

    output = template.render(tables=tables)

    with open(output_path, 'w') as f:
        f.write(output)

def main():
    arg_parser = argparse.ArgumentParser(description='StoredProcedure Generator')
    arg_parser.add_argument('--path', type=str, default='../../TrapperGameServer/GameDB.xml', help='Xml Path')
    arg_parser.add_argument('--output', type=str, default='TestDBToCppGenerator.h', help='Output File')
    args = arg_parser.parse_args()

    tables = parse_xml(args.path)
    generate_cpp_header(tables, 'DBToCpp.h.', args.output)
    print(f'Generated C++ header file: {args.output}')

if __name__ == '__main__':
    main()