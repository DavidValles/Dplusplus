/*
    Main
*/

Blockly.JavaScript['main'] = function(block) {
  var statements_main_method = Blockly.JavaScript.statementToCode(block, 'main_method');
  var code = 'main {\n' + statements_main_method + '}\n';
  return code;
};

/*
    Functions
*/

Blockly.JavaScript['function_with_params'] = function(block) {
  var dropdown_type = block.getFieldValue('TYPE');
  var text_id = block.getFieldValue('ID');
  var value_params = Blockly.JavaScript.valueToCode(block, 'PARAMS', Blockly.JavaScript.ORDER_ATOMIC);
  var statements_name = Blockly.JavaScript.statementToCode(block, 'NAME');
  value_params = value_params.replace(/[()]/g,'');
  var code = 'func '+ dropdown_type + ' ' + text_id + ' (' + value_params +')' + '{\n' + statements_name + '}\n';
  return code;
};

Blockly.JavaScript['function_without_params'] = function(block) {
  var dropdown_type = block.getFieldValue('TYPE');
  var text_id = block.getFieldValue('ID');
  var statements_name = Blockly.JavaScript.statementToCode(block, 'NAME');
  var code = 'func '+ dropdown_type + ' ' + text_id +' (){\n' + statements_name + '\n}\n';
  return code;
};

Blockly.JavaScript['param_continue'] = function(block) {
  var dropdown_type = block.getFieldValue('TYPE');
  var text_name_param = block.getFieldValue('name_param');
  var value_params_continue = Blockly.JavaScript.valueToCode(block, 'params_continue', Blockly.JavaScript.ORDER_ATOMIC);
  var code = dropdown_type + ' ' + text_name_param + ', ' +  value_params_continue;
  return [code, Blockly.JavaScript.ORDER_ATOMIC];
};

Blockly.JavaScript['param_stop'] = function(block) {
  var dropdown_type = block.getFieldValue('TYPE');
  var text_name_param = block.getFieldValue('name_param');
  var code = dropdown_type + ' ' + text_name_param;
  return [code, Blockly.JavaScript.ORDER_NONE];
};

Blockly.JavaScript['functioncall'] = function(block) {
  var text_funcname = block.getFieldValue('FUNCNAME');
  var code = text_funcname + ';\n';
  return code;
};

Blockly.JavaScript['return'] = function(block) {
  var value_return = Blockly.JavaScript.valueToCode(block, 'RETURN', Blockly.JavaScript.ORDER_ATOMIC);
  var code = 'return ' + value_return + ';\n';
  return code;
};

/*
    Variables
*/

Blockly.JavaScript['var_def_stop'] = function(block) {
  var dropdown_type = block.getFieldValue('TYPE');
  var text_id = block.getFieldValue('ID');
  var text_value = block.getFieldValue('VALUE');
  var code = 'var ' + dropdown_type + ' ' + text_id + '=' + text_value +';\n';
  return code;
};

Blockly.JavaScript['var_def_continue'] = function(block) {
  var dropdown_type = block.getFieldValue('TYPE');
  var text_id = block.getFieldValue('ID');
  var text_value = block.getFieldValue('VALUE');
  var value_name = Blockly.JavaScript.valueToCode(block, 'NAME', Blockly.JavaScript.ORDER_ATOMIC);
  var code = 'var ' + dropdown_type + ' ' + text_id + '=' + text_value +',' + value_name;
  return code;
};

Blockly.JavaScript['var_continue'] = function(block) {
  var text_id = block.getFieldValue('ID');
  var text_value = block.getFieldValue('VALUE');
  var value_name = Blockly.JavaScript.valueToCode(block, 'NAME', Blockly.JavaScript.ORDER_ATOMIC);
  var code = text_id + '=' + text_value + ',' + value_name;
  return [code, Blockly.JavaScript.ORDER_ATOMIC];
};

Blockly.JavaScript['var_stop'] = function(block) {
  var text_id = block.getFieldValue('ID');
  var text_value = block.getFieldValue('VALUE');
  var code = text_id + '=' + text_value + ';\n';
  return [code, Blockly.JavaScript.ORDER_ATOMIC];
};

Blockly.JavaScript['var_assignment'] = function(block) {
  var text_id = block.getFieldValue('ID');
  var value_name = Blockly.JavaScript.valueToCode(block, 'NAME', Blockly.JavaScript.ORDER_ATOMIC);
  var code = text_id + '=' + value_name + ';\n';
  return code;
};

Blockly.JavaScript['id_continue'] = function(block) {
  var text_id = block.getFieldValue('ID');
  var value_name = Blockly.JavaScript.valueToCode(block, 'NAME', Blockly.JavaScript.ORDER_ATOMIC);
  var code = text_id + ', ' + value_name;
  return [code, Blockly.JavaScript.ORDER_NONE];
};

Blockly.JavaScript['id_stop'] = function(block) {
  var text_id = block.getFieldValue('ID');
  var code = text_id;
  return [code, Blockly.JavaScript.ORDER_ATOMIC];
};

/*
    Constants
*/

Blockly.JavaScript['const_continue'] = function(block) {
  var number_name = block.getFieldValue('NAME');
  var value_consn = Blockly.JavaScript.valueToCode(block, 'CONST', Blockly.JavaScript.ORDER_ATOMIC);
  var code = number_name + ', ' +  value_consn;
  return [code, Blockly.JavaScript.ORDER_NONE];
};

Blockly.JavaScript['const_stop'] = function(block) {
  var number_consn = block.getFieldValue('CONST');
  var code = number_consn ;
  return [code, Blockly.JavaScript.ORDER_NONE];
};

/*
    Arrays / Matrices
*/

Blockly.JavaScript['array_definition'] = function(block) {
  var dropdown_type = block.getFieldValue('TYPE');
  var text_id = block.getFieldValue('ID');
  var number_size = block.getFieldValue('SIZE');
  var code = dropdown_type +' ' + text_id + '(' + number_size + ');\n';
  return code;
};

Blockly.JavaScript['array_assignment'] = function(block) {
  var text_id = block.getFieldValue('ID');
  var number_place = block.getFieldValue('PLACE');
  var value_name = Blockly.JavaScript.valueToCode(block, 'NAME', Blockly.JavaScript.ORDER_ATOMIC);
  var code = text_id + '[' + number_place + '] = ' + value_name + ';\n';
  return code;
};

Blockly.JavaScript['array_access'] = function(block) {
  var text_id = block.getFieldValue('ID');
  var text_size = block.getFieldValue('SIZE');
  var code = text_id + '[' + text_size + ']'
  return [code, Blockly.JavaScript.ORDER_ATOMIC];
};

Blockly.JavaScript['matrix_definition'] = function(block) {
  var dropdown_type = block.getFieldValue('TYPE');
  var text_id = block.getFieldValue('ID');
  var number_size1 = block.getFieldValue('SIZE1');
  var number_size2 = block.getFieldValue('SIZE2');
  var code = dropdown_type +' ' + text_id + '(' + number_size1 + ',' + number_size2 +');\n';
  return code;
};

Blockly.JavaScript['matrix_assignment'] = function(block) {
  var text_id = block.getFieldValue('ID');
  var number_place1 = block.getFieldValue('INDEX1');
  var number_place2 = block.getFieldValue('INDEX2');
  var value_name = Blockly.JavaScript.valueToCode(block, 'NAME', Blockly.JavaScript.ORDER_ATOMIC);
  var code = text_id + '[' + number_place1 + ']'  + '[' + number_place2 + '] = ' + value_name + ';\n';
  return code;
};

Blockly.JavaScript['matrix_access'] = function(block) {
  var text_id = block.getFieldValue('ID');
  var text_size1 = block.getFieldValue('INDEX1');
  var text_size2 = block.getFieldValue('INDEX2');
  var code = text_id + '[' + text_size1 + ']' + '[' + text_size2 + ']'
  return [code, Blockly.JavaScript.ORDER_ATOMIC];
};

/*
    Read / Print
*/

Blockly.JavaScript['read'] = function(block) {
  var text_id = block.getFieldValue('ID');
  var code = 'read (' + (text_id) + ');\n';
  return code;
};


Blockly.JavaScript['print'] = function(block) {
  var value_value = Blockly.JavaScript.valueToCode(block, 'VALUE', Blockly.JavaScript.ORDER_ATOMIC);
  var code = 'print(' + value_value + ');\n';
  return code;
};
