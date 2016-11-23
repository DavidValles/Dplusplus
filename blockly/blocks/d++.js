'use strict';

goog.provide('Blockly.Blocks.custom');

goog.require('Blockly.Blocks');

/*
    Main
*/
Blockly.Blocks['main'] = {
  init: function() {
    this.appendDummyInput()
        .appendField("Main");
    this.appendStatementInput("main_method")
        .setCheck(null);
    this.setPreviousStatement(true, null);
    this.setColour(300);
    this.setTooltip('');
    this.setHelpUrl('http://www.example.com/');
  }
};

/*
    Functions
*/

Blockly.Blocks['function_with_params'] = {
  init: function() {
    this.appendValueInput("PARAMS")
        .setCheck(null)
        .appendField("func")
        .appendField(new Blockly.FieldDropdown([["none", "none"], ["integer", "integer"], ["decimal", "decimal"], ["character", "character"], ["flag", "flag"]]), "TYPE")
        .appendField(new Blockly.FieldTextInput("id"), "ID");
    this.appendStatementInput("NAME")
        .setCheck(null);
    this.setPreviousStatement(true, null);
    this.setNextStatement(true, null);
    this.setColour(230);
    this.setTooltip('');
    this.setHelpUrl('http://www.example.com/');
  }
};

Blockly.Blocks['function_without_params'] = {
  init: function() {
    this.appendDummyInput()
        .appendField("func")
        .appendField(new Blockly.FieldDropdown([["none", "none"], ["integer", "integer"], ["decimal", "decimal"], ["character", "character"], ["flag", "flag"]]), "TYPE")
        .appendField(new Blockly.FieldTextInput("id"), "ID");
    this.appendStatementInput("NAME")
        .setCheck(null);
    this.setPreviousStatement(true, null);
    this.setNextStatement(true, null);
    this.setColour(230);
    this.setTooltip('');
    this.setHelpUrl('http://www.example.com/');
  }
};

Blockly.Blocks['param_stop'] = {
  init: function() {
    this.appendDummyInput()
         .appendField(new Blockly.FieldDropdown([["integer","integer"], ["decimal", "decimal"], ["character", "character"], ["flag", "flag"]]), "TYPE")
        .appendField(new Blockly.FieldTextInput(""), "name_param");
    this.setOutput(true, null);
    this.setColour(230);
    this.setTooltip('');
    this.setHelpUrl('http://www.example.com/');
  }
};

Blockly.Blocks['param_continue'] = {
  init: function() {
    this.appendValueInput("params_continue")
        .setCheck(null)
        .appendField(new Blockly.FieldDropdown([["integer", "integer"], ["decimal", "decimal"], ["character", "character"], ["flag", "flag"]]), "TYPE")
        .appendField(new Blockly.FieldTextInput(""), "name_param");
    this.setOutput(true, null);
    this.setColour(230);
    this.setTooltip('');
    this.setHelpUrl('http://www.example.com/');
  }
};

Blockly.Blocks['functioncall_params'] = {
  init: function() {
    this.appendValueInput("NAME")
        .setCheck(null)
        .appendField(new Blockly.FieldTextInput("'functionID'"), "FUNCNAME");
    this.setPreviousStatement(true, null);
    this.setNextStatement(true, null);
    this.setColour(230);
    this.setTooltip('');
    this.setHelpUrl('http://www.example.com/');
  }
};

Blockly.Blocks['functioncall_no_param'] = {
  init: function() {
    this.appendDummyInput()
        .appendField(new Blockly.FieldTextInput("'functionID'"), "FUNCNAME");
    this.setPreviousStatement(true, null);
    this.setNextStatement(true, null);
    this.setColour(230);
    this.setTooltip('');
    this.setHelpUrl('http://www.example.com/');
  }
};

Blockly.Blocks['return'] = {
  init: function() {
    this.appendValueInput("RETURN")
        .setCheck(null)
        .appendField("Return");
    this.setPreviousStatement(true, null);
    this.setColour(255);
    this.setTooltip('');
    this.setHelpUrl('http://www.example.com/');
  }
};

/*
    Variables
*/

Blockly.Blocks['var_def_stop'] = {
  init: function() {
    this.appendDummyInput()
        .appendField(new Blockly.FieldDropdown([["integer", "integer"], ["decimal", "decimal"], ["character", "character"], ["flag", "flag"]]), "TYPE")
        .appendField(new Blockly.FieldTextInput("id"), "ID")
        .appendField("=")
        .appendField(new Blockly.FieldTextInput(""), "VALUE");
    this.setPreviousStatement(true, null);
    this.setNextStatement(true, null);
    this.setColour(230);
    this.setTooltip('');
    this.setHelpUrl('http://www.example.com/');
  }
};

Blockly.Blocks['var_def_continue'] = {
  init: function() {
    this.appendValueInput("NAME")
        .setCheck(null)
        .appendField(new Blockly.FieldDropdown([["integer", "integer"], ["decimal", "decimal"], ["character", "character"], ["flag", "flag"]]), "TYPE")
        .appendField(new Blockly.FieldTextInput("id"), "ID")
        .appendField("=")
        .appendField(new Blockly.FieldTextInput(""), "VALUE");
    this.setPreviousStatement(true, null);
    this.setNextStatement(true, null);
    this.setColour(230);
    this.setTooltip('');
    this.setHelpUrl('http://www.example.com/');
  }
};

Blockly.Blocks['var_continue'] = {
  init: function() {
    this.appendValueInput("NAME")
        .setCheck(null)
        .appendField(new Blockly.FieldTextInput("id"), "ID")
        .appendField("=")
        .appendField(new Blockly.FieldTextInput(""), "VALUE");
    this.setOutput(true, null);
    this.setColour(230);
    this.setTooltip('');
    this.setHelpUrl('http://www.example.com/');
  }
};

Blockly.Blocks['var_stop'] = {
  init: function() {
    this.appendDummyInput()
        .appendField(new Blockly.FieldTextInput("id"), "ID")
        .appendField("=")
        .appendField(new Blockly.FieldTextInput(""), "VALUE");
    this.setOutput(true, null);
    this.setColour(230);
    this.setTooltip('');
    this.setHelpUrl('http://www.example.com/');
  }
};

Blockly.Blocks['var_assignment'] = {
  init: function() {
    this.appendValueInput("NAME")
        .setCheck(null)
        .appendField(new Blockly.FieldTextInput("id"), "ID")
        .appendField("=");
    this.setPreviousStatement(true, null);
    this.setNextStatement(true, null);
    this.setColour(230);
    this.setTooltip('');
    this.setHelpUrl('http://www.example.com/');
  }
};

Blockly.Blocks['id_continue'] = {
  init: function() {
    this.appendValueInput("NAME")
        .setCheck(null)
        .appendField(new Blockly.FieldTextInput(""), "ID");
    this.setOutput(true, null);
    this.setColour(330);
    this.setTooltip('');
    this.setHelpUrl('http://www.example.com/');
  }
};

Blockly.Blocks['id_stop'] = {
  init: function() {
    this.appendDummyInput()
        .appendField(new Blockly.FieldTextInput(""), "ID");
    this.setOutput(true, null);
    this.setColour(330);
    this.setTooltip('');
    this.setHelpUrl('http://www.example.com/');
  }
};

/*
   Constants
*/

Blockly.Blocks['const_continue'] = {
  init: function() {
    this.appendValueInput("CONST")
        .setCheck(null)
        .appendField(new Blockly.FieldNumber(0), "NAME");
    this.setOutput(true, null);
    this.setColour(330);
    this.setTooltip('');
    this.setHelpUrl('http://www.example.com/');
  }
};

Blockly.Blocks['const_stop'] = {
  init: function() {
    this.appendDummyInput()
        .appendField(new Blockly.FieldNumber(0), "CONST");
    this.setOutput(true, null);
    this.setColour(330);
    this.setTooltip('');
    this.setHelpUrl('http://www.example.com/');
  }
};

/*
   Arrays and Matrix
*/

Blockly.Blocks['array_definition'] = {
  init: function() {
    this.appendDummyInput()
        .appendField(new Blockly.FieldDropdown([["integer", "integer"], ["decimal", "decimal"], ["character", "character"], ["flag", "flag"]]), "TYPE")
        .appendField(new Blockly.FieldTextInput("id"), "ID")
        .appendField("[")
        .appendField(new Blockly.FieldNumber(0, 1), "SIZE")
        .appendField("]")
    this.setPreviousStatement(true, null);
    this.setNextStatement(true, null);
    this.setColour(225);
    this.setTooltip('');
    this.setHelpUrl('http://www.example.com/');
  }
};

Blockly.Blocks['array_assignment'] = {
  init: function() {
    this.appendValueInput("NAME")
        .setCheck(null)
        .appendField(new Blockly.FieldTextInput("id"), "ID")
        .appendField("[")
        .appendField(new Blockly.FieldTextInput("0"), "PLACE")
        .appendField("]")
        .appendField("=");
    this.setPreviousStatement(true, null);
    this.setNextStatement(true, null);
    this.setColour(225);
    this.setTooltip('');
    this.setHelpUrl('http://www.example.com/');
  }
};

Blockly.Blocks['array_access'] = {
  init: function() {
    this.appendDummyInput()
        .appendField(new Blockly.FieldTextInput("id"), "ID")
        .appendField("[")
        .appendField(new Blockly.FieldTextInput("0"), "SIZE")
        .appendField("]");
    this.setOutput(true, null);
    this.setColour(225);
    this.setTooltip('');
    this.setHelpUrl('http://www.example.com/');
  }
};

Blockly.Blocks['matrix_definition'] = {
  init: function() {
    this.appendDummyInput()
        .appendField(new Blockly.FieldDropdown([["integer", "integer"], ["decimal", "decimal"], ["character", "character"], ["flag", "flag"]]), "TYPE")
        .appendField(new Blockly.FieldTextInput("id"), "ID")
        .appendField("[")
        .appendField(new Blockly.FieldNumber(0, 1), "SIZE1")
        .appendField("]")
        .appendField("[")
        .appendField(new Blockly.FieldNumber(0, 1), "SIZE2")
        .appendField("]")
    this.setPreviousStatement(true, null);
    this.setNextStatement(true, null);
    this.setColour(225);
    this.setTooltip('');
    this.setHelpUrl('http://www.example.com/');
  }
};

Blockly.Blocks['matrix_assignment'] = {
  init: function() {
    this.appendValueInput("NAME")
        .setCheck(null)
        .appendField(new Blockly.FieldTextInput("id"), "ID")
        .appendField("[")
        .appendField(new Blockly.FieldTextInput("0"), "INDEX1")
        .appendField("]")
        .appendField("[")
        .appendField(new Blockly.FieldTextInput("0"), "INDEX2")
        .appendField("]")
        .appendField("=");
    this.setPreviousStatement(true, null);
    this.setNextStatement(true, null);
    this.setColour(225);
    this.setTooltip('');
    this.setHelpUrl('http://www.example.com/');
  }
};

Blockly.Blocks['matrix_access'] = {
  init: function() {
    this.appendDummyInput()
        .appendField(new Blockly.FieldTextInput("id"), "ID")
        .appendField("[")
        .appendField(new Blockly.FieldTextInput("0"), "INDEX1")
        .appendField("]")
        .appendField("[")
        .appendField(new Blockly.FieldTextInput("0"), "INDEX2")
        .appendField("]")
    this.setOutput(true, null);
    this.setColour(225);
    this.setTooltip('');
    this.setHelpUrl('http://www.example.com/');
  }
};

/*
    Read and Print
*/


Blockly.Blocks['read'] = {
  init: function() {
    this.appendDummyInput()
        .appendField("read")
        .appendField(new Blockly.FieldTextInput("id"), "ID")
    this.setPreviousStatement(true, null);
    this.setNextStatement(true, null);
    this.setColour(250);
    this.setTooltip('');
    this.setHelpUrl('http://www.example.com/');
  }
};


Blockly.Blocks['print'] = {
  init: function() {
    this.appendValueInput("VALUE")
        .setCheck(null)
        .appendField("print");
    this.setPreviousStatement(true, null);
    this.setNextStatement(true, null);
    this.setColour(250);
    this.setTooltip('');
    this.setHelpUrl('http://www.example.com/');
  }
};

Blockly.Blocks['parenthesis'] = {
  init: function() {
    this.appendValueInput("VALUE")
        .setCheck(null)
        .appendField("(");
    this.appendDummyInput()
        .appendField(")");
    this.setInputsInline(true);
    this.setOutput(true, null);
    this.setColour(120);
    this.setTooltip('');
    this.setHelpUrl('http://www.example.com/');
  }
};
