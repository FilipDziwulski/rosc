# Software License Agreement (BSD License)
#
# Copyright (c) 2013, Synapticon GmbH
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
#
# * Redistributions of source code must retain the above copyright
# notice, this list of conditions and the following disclaimer.
# * Redistributions in binary form must reproduce the above
# copyright notice, this list of conditions and the following
# disclaimer in the documentation and/or other materials provided
# with the distribution.
# * Neither the name of Synapticon GmbH nor the names of its
# contributors may be used to endorse or promote products derived
# from this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
# FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
# COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
# INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
# BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
# LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
# CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
# LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
# ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
# POSSIBILITY OF SUCH DAMAGE.

import genc
import genc.generator
import genmsg.names
import genmsg.msg_loader
import genmsg.msgs


class msg(object):
    '''
    classdocs
    '''

    __search_path=[]
    __msg_spec=[]
    __msg_buildup=[]
    __msg_static_struct=""
    __msg_static_size_fields=[]
    __array_depth=0
    __max_array_depth=0
    __message_depth=0
    

    def __init__(self, msg_spec, search_path):
        '''
        Constructor
        '''
        self.__msg_spec=msg_spec
        self.__search_path=search_path
        self.__process_spec(msg_spec)
         
    def get_msgspec_form_field(self, field):
        msg_context = genmsg.msg_loader.MsgContext()
        return genmsg.msg_loader.load_msg_by_type(msg_context, field.base_type, self.__search_path)
        
        
    def getBuildupArray(self):
        return self.__msg_buildup
    
    def printBuildupArray(self):
        for (indent,text, additionalreturn) in self.__msg_buildup:
            if additionalreturn == 1:
                print self.add_tabs(indent) + text + ",\n"
            else:
                print self.add_tabs(indent) + text + ","
    
    def printStaticMsgStruct(self):
        print self.__msg_static_struct
    
    def printStaticMsgStructInit(self):
        print self.__msg_static_size_fields
        
    def printStaticMsgStructInitDefinition(self):
        output= "#define ROSC_USERDEF_MSG_" + self.__msg_spec.package + self.__msg_spec.short_name + "(\n"
        for staticf in self.__msg_static_size_fields:
            if not (staticf.isdigit()):
                output+=staticf + ",\n"
        output+=")"
        output+=self.__msg_static_struct.replace("\n", "\\\n")
        print output
        pass
    
    def add_tabs(self, count):
        tabs=""
        for i in range(count):
            tabs = tabs + "\t"
        return tabs

    def __addBuildupEntry(self,field):
        entry=""
        if field.base_type not in ['byte','char','bool','uint8','int8','uint16','int16','uint32','int32','uint64','int64','float32','float64','string','time','duration']:
            entry="ROS_MSG_BUILDUP_TYPE_SUBMESSAGE"
            if(field.is_array):
                entry+="ARRAY"
                if(field.array_len == None): #array with undefined length
                    entry+="_UL"
            self.__msg_buildup.append((self.__message_depth, entry, 0))
        else:
            indent=self.__message_depth
            if(field.is_array):
                indent+=1
                entry="ROS_MSG_BUILDUP_TYPE_ARRAY"
                if(field.array_len == None): #array with undefined length
                    entry+="_UL"
                self.__msg_buildup.append((self.__message_depth, entry, 0))
                self.__msg_buildup.append((self.__message_depth+1, "ROS_MSG_BUILDUP_TYPE_" + field.base_type.upper(), 1))
            else:
                self.__msg_buildup.append((self.__message_depth, "ROS_MSG_BUILDUP_TYPE_" + field.base_type.upper(), 1))

    
    def __addMessageHeader(self, prev_field, prev_names):
        indent=""
        for i in range(self.__message_depth):
            indent+="\t"
        self.__msg_static_struct+=indent + "struct\n"
        self.__msg_static_struct+=indent + "{\n"
        if(self.__message_depth != 0 and prev_field.is_array):
            self.__msg_static_size_fields.append(self.__genMessageLengthOrDefineString(prev_field, prev_names))
    
    
    def __genMessageLengthOrDefineString(self, prev_field, prev_names):
        lendef=""
        if(self.__message_depth !=0):
            if (prev_field.is_array):
                if(prev_field.array_len == None):
                    lendef+='MAX_SIZE_ARRAY' + prev_names
                else:
                    lendef=str(prev_field.array_len)
        return lendef
    
    def __addMessageFooter(self, prev_field, prev_names):
        indent=""
        for i in range(self.__message_depth):
            indent+="\t"
        if(self.__message_depth ==0):
            self.__msg_static_struct+=indent + "}"
        else:
            self.__msg_static_struct+=indent + "}" + prev_field.name
            if (prev_field.is_array):
                self.__msg_static_struct+="["
                self.__msg_static_struct+=self.__genMessageLengthOrDefineString(prev_field, prev_names)
                self.__msg_static_struct+="]"
            self.__msg_static_struct+=";\n"

    def __addVariable(self, field, prev_names):
        footer=""
        self.__message_depth+=1
        if(field.is_array):
            self.__addMessageHeader(field, prev_names + "_" + field.name)
            self.__msg_static_struct+= self.add_tabs(self.__message_depth+1) + 'uint32_t size;' + '\n'
            if(field.array_len == None):
                self.__msg_static_struct+= self.add_tabs(self.__message_depth+1) + 'bool oversize;' + '\n'
            self.__message_depth+=1
            
            if(field.base_type !="string"):
                field_out="data"
            else:
                field_out=field.name
        else:
            field_out=field.name
            
        if field.base_type in ['uint8', 'int8', 'uint16', 'int16', 'uint32', 'int32', 'uint64', 'int64', 'float32', 'float64']:
            self.__msg_static_struct += (self.add_tabs(self.__message_depth) + field.base_type + "_t " + field_out)
        elif field.base_type == 'byte':
           self.__msg_static_struct += (self.add_tabs(self.__message_depth) + 'uint8_t ' + field_out)
        elif field.base_type == 'bool':
           self.__msg_static_struct += (self.add_tabs(self.__message_depth) + 'bool ' + field_out)
        elif field.base_type == 'char':
           self. __msg_static_struct += (self.add_tabs(self.__message_depth) + 'int8_t ' + field_out)
        elif field.base_type == 'time' or  field.base_type == 'duration':
           output= self.add_tabs(self.__message_depth) + 'struct\n'
           output+= self.add_tabs(self.__message_depth) + '{' + '\n'
           output+= self.add_tabs(self.__message_depth+1) + 'uint32_t sec;' + '\n'
           output+= self.add_tabs(self.__message_depth+1) + 'uint32_t nsec;' + '\n'
           output+= self.add_tabs(self.__message_depth) + '}' + field_out
           self.__msg_static_struct+=output
        elif field.base_type == 'string':
           output= self.add_tabs(self.__message_depth) + 'struct\n'
           output+= self.add_tabs(self.__message_depth) + '{' + '\n'
           output+= self.add_tabs(self.__message_depth+1) + 'uint32_t size;' + '\n'
           output+= self.add_tabs(self.__message_depth+1) + 'bool oversize;' + '\n'
           output+= self.add_tabs(self.__message_depth+1) + 'char data['  "MAX_SIZE_STRING" + prev_names + "_" + field_out +'];' + '\n' 
           output+= self.add_tabs(self.__message_depth) + '}' + field_out
           self.__msg_static_size_fields.append("MAX_SIZE_STRING" + prev_names + "_" + field_out)
           self.__msg_static_struct+=output 
        self.__msg_static_struct+= ";\n"
        
        if(field.is_array):
            self.__message_depth-=1
            self.__addMessageFooter(field, prev_names + "_" + field.name)
            pass
        
        self.__message_depth-=1

        
        
    def __process_spec(self, spec, prev_field=None, prev_names=''):
        self.__addMessageHeader(prev_field, prev_names)
        for field in spec.parsed_fields():
            self.__addBuildupEntry(field)
            if field.base_type not in ['byte','char','bool','uint8','int8','uint16','int16','uint32','int32','uint64','int64','float32','float64','string','time','duration']:
                               
                #####ARRAY DEPTH SIZE DETERMINATION#####
                if (field.is_array):
                    self.__array_depth+=1
                    if (self.__array_depth+1 > self.__max_array_depth):
                        self.__max_array_depth+=1
                ########################################
                

                self.__message_depth+=1
                self.__process_spec(self.get_msgspec_form_field(field), field, prev_names + "_" + field.name) ##Recursive call go one message type deeper
                self.__message_depth-=1
                
                #####ARRAY DEPTH SIZE DETERMINATION#####
                if(field.is_array):
                    self.__array_depth-=1
                ########################################   
            else:
                self.__addVariable(field, prev_names)
                if(field.is_array):
                    
                    #####ARRAY DEPTH SIZE DETERMINATION###################
                    if(field.base_type == 'string'): #Stringarray ...
                        if (self.__array_depth+2 >self.__max_array_depth):
                            self.__max_array_depth=self.__array_depth+2
                    else:
                        if (self.__array_depth+1 >self.__max_array_depth):
                            self.max_array_depth=self.__array_depth+1
                    #######################################################

                               
                #####ARRAY DEPTH SIZE DETERMINATION###################               
                if(field.base_type == 'string'):#String type, which is actually a array itself
                    if (self.__array_depth+1 >self.__max_array_depth):
                        self.max_array_depth=self.__array_depth+1
                #######################################################

        self.__msg_buildup.append((self.__message_depth,'ROS_MSG_BUILDUP_TYPE_MESSAGE_END',1))

        self.__addMessageFooter(prev_field, prev_names)
        if(self.__message_depth==0):
            
            self.__msg_static_struct+="rosc_static_msg" + "_" + spec.package + "__" +spec.short_name + "_ ## USER_TYPENAME"
        pass
