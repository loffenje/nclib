#ifndef TYPES_H
#define TYPES_H

#define any_wrap(Any, value, type) wrap_##type(Any, value)

typedef enum Type {
    I8,U8,I16,U16,I32,U32,I64,U64,F32,F64,
    NSTRING,BLIST,PTR,NMAP
} Type;


typedef struct Any {
    Type _type;

    union {
        int8_t i8;
        uint8_t u8;
        int16_t i16;
        uint16_t u16;
        int32_t i32;
        uint32_t u32;
        int64_t i64;
        uint64_t u64;
        double f64;
        float f32;
        uintptr_t ptr;
        NString nstr;
        BList blist;
        NMap nmap;
    } _data;

} Any;

extern inline void wrap_int8_t(Any *any, int8_t value) {
    any->_data.i8 = value;
    any->_type = I8;
}

extern inline void wrap_uint8_t(Any *any, uint8_t value) {
    any->_data.u8 = value;
    any->_type = U8;
}

extern inline void wrap_int16_t(Any *any, int16_t value) {
    any->_data.i16 = value;
    any->_type = I16;
}

extern inline void wrap_uint16_t(Any *any, uint16_t value) {
    any->_data.u16 = value;
    any->_type = U16;
}

extern inline void wrap_int32_t(Any *any, int32_t value) {
    any->_data.i32 = value;
    any->_type = I32;
}

extern inline void wrap_uint32_t(Any *any, uint32_t value) {
    any->_data.u32 = value;
    any->_type = U32;
}

extern inline void wrap_int64_t(Any *any, int64_t value) {
    any->_data.i64 = value;
    any->_type = I64;
}

extern inline void wrap_uint64_t(Any *any, uint64_t value) {
    any->_data.u64 = value;
    any->_type = U64;
}

extern inline void wrap_uintptr_t(Any *any, uintptr_t value) {
    any->_data.ptr = value;
    any->_type = PTR;
}

extern inline void wrap_float(Any *any, float value) {
    any->_data.f32 = value;
    any->_type = F32;
}

extern inline void wrap_double(Any *any, double value) {
    any->_data.f64 = value;
    any->_type = F64;
}

extern inline void wrap_NString(Any *any, NString *value) {
    any->_data.nstr = *value;
    any->_type = NSTRING;
}

extern inline void wrap_BList(Any *any, BList *value) {
    any->_data.blist = *value;
    any->_type = BLIST;
}

extern inline void wrap_NMap(Any *any, NMap *value) {
    any->_data.nmap = *value;
    any->_type = NMAP;
}

extern inline void wrap_int(Any *any, int value) {
    wrap_int32_t(any, (int32_t)value);
}

extern inline void wrap_char(Any *any, char value) {
    wrap_int8_t(any, (int8_t)value);
}

void any_unwrap(Any *any, void *out) {
    switch (any->_type) {
        case I8:
            *(int8_t *)out = any->_data.i8;
            break;
        case U8:
            *(uint8_t *)out = any->_data.u8;
            break;
        case I16:
            *(int16_t *)out = any->_data.i16;
            break;
        case U16:
            *(uint16_t *)out = any->_data.u16;
            break;
        case I32:
            *(int32_t *)out = any->_data.i32;
            break;
        case U32:
            *(uint32_t *)out = any->_data.u32;
            break;
        case I64:
            *(int64_t *)out = any->_data.i64;
            break;
        case U64:
            *(uint64_t *)out = any->_data.u64;
            break;
        case F32:
            *(float *)out = any->_data.f32;
            break;
        case F64:
            *(double *)out = any->_data.f64;
            break;
        case PTR:
            *(uintptr_t *)out = any->_data.ptr;
            break;
        case NSTRING:
            *(NString *)out = any->_data.nstr;
            break;
        case BLIST:
            *(BList *)out = any->_data.blist;
            break;
        case NMAP:
            *(NMap *)out = any->_data.nmap;
            break;
    }
}

#endif
