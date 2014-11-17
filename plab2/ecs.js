#!/usr/bin/env node

var fs = require('fs');
var prog = require('commander');
var setcase = require('change-case');

/*
 * see ./ecs.js --help
 */

prog
    .option('-n, --name [name_with_snake_case]', 'Name for the specified component and/or system')
    .option('-a, --add', 'Add system and/or component')
    .option('-r, --remove', 'Remove system and/or component')
    .option('-c, --component', 'Process a component')
    .option('-s, --system', 'Process a system')
    //TODO optimization for later...
    //.option('-S, --rendersystem', 'Process a render system')
    .parse(process.argv);

if ( prog.add === undefined && prog.remove === undefined ) {
    console.log('error: pick --add OR --remove / -h for help');
    return;
}

if ( prog.add !== undefined && prog.remove !== undefined ) {
    console.log('error: pick --add OR --remove not both / -h for help');
    return;
}

if ( prog.component === undefined && prog.system === undefined /*&& prog.rendersystem === undefined*/ ) {
    prog.component = true;
    prog.system = true;
    //console.log('error: pick --component AND/OR --system AND/OR --rendersystem / -h for help');
    return;
}

var _name = prog.name;

if ( typeof _name !== 'string' ) {
    console.log('error: --name is',_name,' / -h for help');
    return;
}

var code_placeholder = '//dont remove this line';

var name_pascalcase = setcase.pascalCase(_name);
var name_snakecase = setcase.snakeCase(_name);

console.log(' - component name:', _name);
console.log(' - pascal case:', name_pascalcase);
console.log(' - snake case:', name_snakecase);

// files

// component

var comp_filename = name_pascalcase+'Component.h';
var comp_path = __dirname + '/src/components/'+ comp_filename;

var comp_include_path = __dirname + '/src/components/Components.h';
var comp_include_code = '#include "components/'+ name_pascalcase +'Component.h"' + '\n';

var comp_tpl_path = __dirname + '/src/components/TemplateComponent.h';

var comp_factory_path = __dirname + '/src/components/PlabComponentFactory.h'; 
var comp_factory_code = 'else if (id == "'+ name_snakecase +'") return new '+ name_pascalcase +'Component(id);' + '\n';


// system

var sys_filename = name_pascalcase+'System.h';
var sys_path = __dirname + '/src/systems/'+ sys_filename;

var sys_include_path = __dirname + '/src/systems/Systems.h';
var sys_include_code = '#include "systems/'+ name_pascalcase +'System.h"' + '\n';

var sys_tpl_path = __dirname + '/src/systems/TemplateSystem.h';

var sys_factory_path = __dirname + '/src/systems/PlabSystemFactory.h'; 
var sys_factory_code = 'ecs.add_system(new '+ name_pascalcase +'System("'+name_snakecase+'"));' + '\n';


// render system
//TODO optimization for later...

//var sys_render_filename = name_pascalcase+'RenderSystem.h';
//var sys_render_path = __dirname + '/src/systems/'+ sys_render_filename;

//var sys_render_include_path = sys_include_path; //__dirname + '/src/systems/Systems.h';
//var sys_render_include_code = '#include "systems/'+ name_pascalcase +'RenderSystem.h"' + '\n';

//var sys_render_tpl_path = sys_tpl_path; //__dirname + '/src/systems/TemplateSystem.h';

//var sys_render_factory_path = sys_factory_path; //__dirname + '/src/systems/PlabSystemFactory.h'; 
//var sys_render_factory_code = 'ecs.add_render_system(new '+ name_pascalcase +'RenderSystem("'+name_snakecase+'"));' + '\n';


// functions

function remove_file( path ) {

    if ( ! fs.existsSync(path) ) {
        console.log('remove_file: file not found',path);
        return;
    }

    fs.unlink( path, function (err) {
        if (err) throw err;
        console.log('successfully deleted',path);
    });
}

function remove_code_from_file( path, code ) {

    if ( ! fs.existsSync(path) ) {
        console.log('remove_code_from_file: file not found',path);
        return;
    }

    fs.readFile(path, function(err,data) {
        if (err) throw err;
        if (data.toString().indexOf( code ) == -1 ) {
            console.log('no includes found on',path);
            return;
        }

        var newdata = data.toString().replace(code,'');

        fs.writeFile( path, newdata, function(err) { 
            if (err) throw err;
            console.log('removed code in',path);
        });
    });
}

function make_file_from_template( tpl_path, file_path, pascalcase ) {

    fs.readFile( tpl_path, function (err, data) {
        if (err) throw err;

        if (fs.existsSync(file_path)) {
            console.log('[warning] make_file_from_template:',file_path,'already exists');
            return;
        }

        //for comp
        //var filecontent = data.toString().replace( /Template/g, pascalcase);
        //for sys
        var filecontent = data.toString().replace( /Template/g, pascalcase).replace(/template/g, name_snakecase);

        fs.writeFile( file_path, filecontent, function (err) {
            if (err) throw err;
            console.log(file_path,'made succesfuly');
        });

    });
}

function add_code_to_file( include_path, include_code ) {

    fs.readFile( include_path, function (err, data) {
        if (err) throw err;

        if ( data.toString().indexOf( include_code ) > -1 ) {
            console.log('[warning] add_code_to_file: code for',_name,'already included in',include_path);
            return;
        }

        var newdata = data.toString().replace(code_placeholder, include_code + code_placeholder);

        fs.writeFile( include_path, newdata, function(err) { 
            if (err) throw err;
            console.log('code for',_name,'included in',include_path);
        });

    });
}


// process...

// Remove

if ( prog.remove !== undefined ) {

    if ( prog.component !== undefined ) {
        remove_file( comp_path ); 
        remove_code_from_file( comp_include_path, comp_include_code );
        remove_code_from_file( comp_factory_path, comp_factory_code ); 
    }

    if ( prog.system !== undefined ) {
        remove_file( sys_path );  
        remove_code_from_file( sys_include_path, sys_include_code ); 
        remove_code_from_file( sys_factory_path, sys_factory_code ); 
    }  

    //if ( prog.rendersystem !== undefined ) {
        //remove_file( sys_render_path );  
        //remove_code_from_file( sys_render_include_path, sys_render_include_code ); 
        //remove_code_from_file( sys_render_factory_path, sys_render_factory_code ); 
    //}

}


// Add

else if ( prog.add !== undefined ) {

    if ( prog.component !== undefined ) {

        // copia ./src/components/TemplateComponent.h a ./src/components/MiXxxCsComponent.h
        // en MiXxxCsComponent.h reemplaza: 
        // * TemplateComponent por MiXxxCsComponent

        make_file_from_template( comp_tpl_path, comp_path, name_pascalcase ); 
        add_code_to_file( comp_include_path, comp_include_code ); 
        add_code_to_file( comp_factory_path, comp_factory_code ); 
    }

    if ( prog.system !== undefined ) {

        // copia ./src/systems/TemplateSystem.h a ./src/systems/MiXxxCsSystem.h
        // en MiXxxCsSystem.h reemplaza:
        // * TemplateComponent por MiXxxCsComponent
        // * TemplateSystem por MiXxxCsSystem
        // * template_m por mi_xxx_cs_m

        make_file_from_template( sys_tpl_path, sys_path, name_pascalcase ); 
        add_code_to_file( sys_include_path, sys_include_code ); 
        add_code_to_file( sys_factory_path, sys_factory_code ); 
    }

    //if ( prog.rendersystem !== undefined ) {
        //make_file_from_template( sys_render_tpl_path, sys_render_path, name_pascalcase + 'Render' ); 
        //add_code_to_file( sys_render_include_path, sys_render_include_code ); 
        //add_code_to_file( sys_render_factory_path, sys_render_factory_code ); 
    //}
}

