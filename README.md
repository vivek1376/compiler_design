

## misc shell cmds
`cat tokens | xargs echo | tr , '\n' | xargs -I {} echo {} | sed -nr 's/(.*)/{tokenType::\1, "\1"},/p'`
