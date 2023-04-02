

## misc shell cmds
`cat tokens | xargs echo | tr , '\n' | xargs -I {} echo {} | sed -nr 's/(.*)/{tokenType::\1, "\1"},/p'`
echo * | sed -nr 's/(.*);/\1 : public nt_retType {\npublic:\n\n};\n/p'

## NOTES
commit a4aa3a6 is confusing; file incorrectly added

### nt_ classes
```
class nt_retType_program_body;
class nt_retType_program_identifier;
class nt_retType_program_body;
class nt_retType_declaration;
class nt_retType_statement;
class nt_retType_procedure_declaration;
class nt_retType_variable_declaration;
class nt_retType_procedure_header;
class nt_retType_procedure_body;
class nt_retType_identifier;
class nt_retType_type_mark;
class nt_retType_parameter_list;
class nt_retType_parameter;
class nt_retType_bound;
class nt_retType_number;
class nt_retType_assignment_statement;
class nt_retType_if_statement;
class nt_retType_loop_statement;
class nt_retType_return_statement;
class nt_retType_procedure_call;
class nt_retType_argument_list;
class nt_retType_destination;
class nt_retType_expression;
class nt_retType_arithOp;
class nt_retType_expression_;
class nt_retType_relation;
class nt_retType_arithOp_;
class nt_retType_term;
class nt_retType_factor;
class nt_retType_relation_;
class nt_retType_term_;
class nt_retType_name;
class nt_retType_string;
```
