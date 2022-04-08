# ICPC Templates

ICPC 比赛用模板及自动生成工具。

## 使用方法

目录 `code/` 下的 `_config.yml` 配置生成的文档中的所有 section 及其对应的代码目录。

每个 section 对应目录下的 `_config.yml` 配置对应章节下的 subsection 和（或） subsubsection。

准备好代码后进入 `generator/` 目录，运行

```sh
chmod +x gen.py
./gen.py

```

然后用 `xelatex` 编译两遍 `template.tex` 即可。

如需修改生成 PDF 的样式，请修改 `generator/template.tex`。
