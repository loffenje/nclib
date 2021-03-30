#ifndef TYPES_H
#define TYPES_H

#define wrap(Dynamic, value, type) wrap_##type(Dynamic, value)

typedef enum Type {
    I8,U8,I16,U16,I32,U32,I64,U64,F32,F64,
    NSTRING,BLIST,PTR,NMAP
} Type;


typedef struct Dynamic {
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

} Dynamic;

extern inline void wrap_int8_t(Dynamic *dynamic, int8_t value) {
    dynamic->_data.i8 = value;
    dynamic->_type = I8;
}

extern inline void wrap_uint8_t(Dynamic *dynamic, uint8_t value) {
    dynamic->_data.u8 = value;
    dynamic->_type = U8;
}

extern inline void wrap_int16_t(Dynamic *dynamic, int16_t value) {
    dynamic->_data.i16 = value;
    dynamic->_type = I16;
}

extern inline void wrap_uint16_t(Dynamic *dynamic, uint16_t value) {
    dynamic->_data.u16 = value;
    dynamic->_type = U16;
}

extern inline void wrap_int32_t(Dynamic *dynamic, int32_t value) {
    dynamic->_data.i32 = value;
    dynamic->_type = I32;
}

extern inline void wrap_uint32_t(Dynamic *dynamic, uint32_t value) {
    dynamic->_data.u32 = value;
    dynamic->_type = U32;
}

extern inline void wrap_int64_t(Dynamic *dynamic, int64_t value) {
    dynamic->_data.i64 = value;
    dynamic->_type = I64;
}

extern inline void wrap_uint64_t(Dynamic *dynamic, uint64_t value) {
    dynamic->_data.u64 = value;
    dynamic->_type = U64;
}

extern inline void wrap_uintptr_t(Dynamic *dynamic, uintptr_t value) {
    dynamic->_data.ptr = value;
    dynamic->_type = PTR;
}

extern inline void wrap_float(Dynamic *dynamic, float value) {
    dynamic->_data.f32 = value;
    dynamic->_type = F32;
}

extern inline void wrap_double(Dynamic *dynamic, double value) {
    dynamic->_data.f64 = value;
    dynamic->_type = F64;
}

extern inline void wrap_NString(Dynamic *dynamic, NString *value) {
    dynamic->_data.nstr = *value;
    dynamic->_type = NSTRING;
}

extern inline void wrap_BList(Dynamic *dynamic, BList *value) {
    dynamic->_data.blist = *value;
    dynamic->_type = BLIST;
}

extern inline void wrap_NMap(Dynamic *dynamic, NMap *value) {
    dynamic->_data.nmap = *value;
    dynamic->_type = NMAP;
}

extern inline void wrap_int(Dynamic *dynamic, int value) {
    wrap_int32_t(dynamic, (int32_t)value);
}

extern inline void wrap_char(Dynamic *dynamic, char value) {
    wrap_int8_t(dynamic, (int8_t)value);
}

void unwrap(Dynamic *dynamic, void *out) {
    switch (dynamic->_type) {
        case I8:
            *(int8_t *)out = dynamic->_data.i8;
            break;
        case U8:
            *(uint8_t *)out = dynamic->_data.u8;
            break;
        case I16:
            *(int16_t *)out = dynamic->_data.i16;
            break;
        case U16:
            *(uint16_t *)out = dynamic->_data.u16;
            break;
        case I32:
            *(int32_t *)out = dynamic->_data.i32;
            break;
        case U32:
            *(uint32_t *)out = dynamic->_data.u32;
            break;
        case I64:
            *(int64_t *)out = dynamic->_data.i64;
            break;
        case U64:
            *(uint64_t *)out = dynamic->_data.u64;
            break;
        case F32:
            *(float *)out = dynamic->_data.f32;
            break;
        case F64:
            *(double *)out = dynamic->_data.f64;
            break;
        case PTR:
            *(uintptr_t *)out = dynamic->_data.ptr;
            break;
        case NSTRING:
            *(NString *)out = dynamic->_data.nstr;
            break;
        case BLIST:
            *(BList *)out = dynamic->_data.blist;
            break;
        case NMAP:
            *(NMap *)out = dynamic->_data.nmap;
            break;
    }
}

#endif
