

## misc shell cmds
`cat tokens | xargs echo | tr , '\n' | xargs -I {} echo {} | sed -nr 's/(.*)/{tokenType::\1, "\1"},/p'`
echo * | sed -nr 's/(.*);/\1 : public nt_retType {\npublic:\n\n};\n/p'

## NOTES
commit a4aa3a6 is confusing; file incorrectly added
