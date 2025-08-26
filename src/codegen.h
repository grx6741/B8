#pragma once

#include "ht.h"
#include "ast.h"

#include <stdint.h>
#include <stdio.h>

#define MAX_SCOPE_DEPTH 255

typedef struct
{
    // will hold the offset of all variables declared in this scope
    ht_t addr_table;

    // When this scope is entered, the stack pointer is saved in this frame pointer. when leaving
    // this scope we restore the stack pointer with this
    uint8_t frame_pointer;
} scope_t;

typedef struct
{
    scope_t scope_stack[MAX_SCOPE_DEPTH];
    uint32_t scope_stack_top;

    uint8_t stack_pointer;
} codegen_context_t;

void Codegen( codegen_context_t* ctx, FILE* out_stream, const ast_node_t* AST );
