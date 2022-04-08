#! /usr/bin/python3

import os
import yaml


CODE_DIR = '../code'


def path_join(a, b):
    return os.path.join(a, b)


def load_config(path):
    file_name = path_join(path, "_config.yml")
    with open(file_name, 'r', encoding='utf-8') as f:
        data = yaml.load(f, yaml.FullLoader)
    return data


def get_content(info, section_dir, content):
    before = info.get('before', None)
    code = info.get('code', None)
    after = info.get('after', None)

    if before is not None:
        content.append(F"\n\\input{{{ path_join(section_dir, before) }}}")
    if code is not None:
        content.append(F"\\lstinputlisting{{{ path_join(section_dir, code) }}}")
    if after is not None:
        content.append(F"\n\\input{{{ path_join(section_dir, after) }}}")


def main():
    content = []

    for section in load_config(CODE_DIR)['sections']:
        section_title = section['title']
        section_dir = path_join(CODE_DIR, section['dir'])
        content.append(F"\\section{{{ section_title }}}")

        for subsection in load_config(section_dir)['subsections']:
            subsection_title = subsection['title']
            content.append(F"\\subsection{{{ subsection_title }}}")

            if subsection.get('subsubsections', None) is not None:
                for subsub in subsection['subsubsections']:
                    subsub_title = subsub['title']
                    content.append(F"\\subsubsection{{{ subsub_title }}}")
                    get_content(subsub, section_dir, content)
            else:
                get_content(subsection, section_dir, content)

    with open('content.tex', 'w', encoding='utf-8') as f:
        f.write("\n".join(content))


if __name__ == "__main__":
    main()
