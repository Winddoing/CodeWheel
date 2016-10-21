
############################################################################
file_head_template = \
'''/********************************************************************
 * Copyright (c) 2013 XXXXXXXXX.inc
 * All rights reserved.
 *
 * \\file    $file
 * \\brief   $brief
 * \\version 1.0
 * \\author  winddoing
 * \\Email   winddoing@sina.cn
 * \\date    $date
 ********************************************************************
 */
'''
############################################################################

import sys, os
from time import *
from re import *

#------------------------------------------------
ident_reg_ex = '[A-Za-z_]\w*'

mem_func_reg_ex = '^([\sA-Za-z_\*&amp;:]*)\\b(' + ident_reg_ex + ')\s*::\s*(~?' + ident_reg_ex + ')\s*\((.*)\)\s*(?:const|:)?\s*$'

gb_func_reg_ex = '^\s*(?:static\s+)?([A-Za-z_][\sA-Za-z_\*&amp;:]*)\\b(' + ident_reg_ex + ')\s*\((.*)\)\s*(?:const)?\s*$'

key_words = ['do', 'if', 'return', 'typedef', 'double', 'short', 'bool', 'int', 'signed', 'break', 'else', 'long', 'sizeof', 'union', 'case', 'enum', 'static', 'unsigned', 'catch', 'namespace', 'using', 'char', 'export', 'new', 'struct', 'virtual', 'class', 'extern', 'operator', 'switch', 'void', 'const', 'false', 'private', 'template', 'float', 'protected', 'this', 'continue', 'for', 'public', 'throw', 'while', 'friend', 'true']
#------------------------------------------------
# utility functions

def read_file_lines(file_name) :
    if not os.path.exists(file_name) :
        print 'WARN: ' + file_name + ' is not exist !'
        return []
    f = open(file_name, 'r')
    lines = f.readlines()
    f.close()
    return lines

def write_file_lines(file_name, lines) :
    #file_name = 'new_' + file_name
    f = open(file_name, 'w')
    f.writelines(lines)
    f.close()

def get_date() :
    return strftime('%Y-%m-%d', localtime())

#------------------------------------------------

def make_file_head(file_name) :
    file_name = file_name.split(os.path.sep)[-1]

    t = file_head_template
    t = t.replace('$file', file_name)
    t = t.replace('$date', get_date())

    file_type = file_name.split('.')[-1]
    brief = ''
    if file_type == 'h' or file_type == 'hpp':
        brief = 'define class ??????'
    elif file_type == 'cpp' or file_type == 'c':
        brief = 'implement functions'
    else :
        pass
    t = t.replace('$brief', brief)

    return [t]

def make_func_comment(func_type, class_name, func_name, arg_list, ret_type) :
    text = '/**\n'

    if func_type == 'mem_func' :
        text += ' * %s::%s\n' % (class_name, func_name)
    elif func_type == 'gb_func' :
        text += ' * %s\n' % func_name

    # add brief field
    text += ' * \\brief   '
    if func_type == 'mem_func' :
        if class_name == func_name :
            text += ('constructor of ' + class_name)
        elif func_name == ('~' + class_name) :
            text += ('destructor of ' + class_name)
        elif func_name == 'instance' :
            text += ('singleton instance function of ' + class_name)
        else :
            pass
    text += '\n'

    # add param list
    for arg in arg_list :
        text += ' * \\param   %s\n' % arg

    # add return type
    if ret_type and ret_type != '' and ret_type != 'void' :
        text += ' * \\return  %s\n' % ret_type
    text += ' */\n'

    return [text]

def get_func_arg_list(arg_str) :
    arg_list = []

    for arg in arg_str.split(',') :
        m = match('.*\\b(%s)' % ident_reg_ex, arg.strip())
        if m :
            arg_list.append(m.group(1))

    return arg_list

def get_func_in_line(line) :
    class_name, func_name, arg_list, return_type = '','', [], ''
    func_type = ''

    m = match(mem_func_reg_ex, line)
    if m :
        class_name = m.group(2)
        return_type = m.group(1).strip()
        func_name = m.group(3)
        arg_list = get_func_arg_list(m.group(4))
        func_type = 'mem_func'
    else :
        m = match(gb_func_reg_ex, line)
        if m :
            return_type = m.group(1).strip()
            func_name = m.group(2)
            arg_list = get_func_arg_list(m.group(3))
            func_type = 'gb_func'

    if (class_name in key_words) or (func_name in key_words) :
        func_type = ''

    return  func_type, class_name, func_name, arg_list, return_type

def is_comment_start(line) :
    # find 'xxx /* yyyy ' in line
    return search('/\*', line) != None

def is_comment_end(line) :
    # find ' xxxxx */ ' in line
    return search('\*/', line) != None

def is_need_insert_file_head(lines) :
    if len(lines)  < 2 :
        return True
    for i in range(2) :
        if is_comment_start(lines[i]) :
            return False
    return True

def is_need_insert_comment(lines, index) :
    if index == 0 :
        return True
    else :
        return not is_comment_end(lines[index-1])

def do_source_file(file_name) :
    lines_in = read_file_lines(file_name)
    lines_out = []

    # add file head
    if is_need_insert_file_head(lines_in) :
        lines_out += make_file_head(file_name)

    index = 0
    is_in_comment = False

    for line in lines_in :
        # check start or end line of comment
        if is_comment_start(line) : is_in_comment = True
        if is_comment_end(line) : is_in_comment = False
        # why not if .. elif ... ?
        # because '/*xxxx*/' is a whole comment. so comment head and tail
        # might in the same line.

        # we should ignore the functions inside of comment
        if not is_in_comment :
            t, c, f, a, r = get_func_in_line(line)
            if t != '':
                # check wether function need comment
                if is_need_insert_comment(lines_in, index) :
                    lines_out += make_func_comment(t, c, f, a, r)
            else :
                pass
        lines_out.append(line)
        index += 1

    write_file_lines(file_name, lines_out)

def do_head_file(file_name) :
    lines_in = read_file_lines(file_name)
    lines_out = []

    if is_need_insert_file_head(lines_in) :
        lines_out += make_file_head(file_name)

    lines_out += lines_in
    write_file_lines(file_name, lines_out)

def do_file() :
    for file_name in sys.argv :
        if file_name == sys.argv[0] :
            continue
        file_type = file_name.split('.')[-1]
        if file_type == 'cpp' or file_type == 'c':
            do_source_file(file_name)
        elif file_type == 'h' or file_type == 'hpp' :
            do_head_file(file_name)

#######################################################################
if __name__ == '__main__' :
    do_file()
