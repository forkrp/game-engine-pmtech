#ifndef _pen_json_h
#define _pen_json_h

#include "pen.h"
#include "jsmn/jsmn.h"
#include "str/Str.h"
#include "hash.h"

namespace pen
{
    struct json_object;
    
    class json
    {
    public:
        ~json();
        json( );
        json( const json& other );
        
        static json load_from_file( const c8* filename );
        static json load( const c8* json_str );
        static json combine( const json& j1, const json& j2, s32 indent = 0 );
        
        Str         dumps();
        Str         name();
        jsmntype_t  type();
        u32         size() const;
        
        json operator [] (const c8* name) const;
        json operator [] (const u32 index) const;
        json operator [] (const s32 index) const;
        json& operator = (const json& other);
        
        Str         as_str( const c8* default_value = nullptr );
        const c8*   as_cstr( const c8* default_value = nullptr );
        hash_id     as_hash_id( hash_id default_value = 0 );
        u32         as_u32( u32 default_value = 0 );
        s32         as_s32( s32 default_value = 0 );
        bool        as_bool( bool default_value = false );
        f32         as_f32( f32 default_value = 0.0f );
        u8          as_u8_hex( u8 default_value = 0 );
        u32         as_u32_hex( u32 default_value = 0 );

        void    set(const c8* name, const Str val);
        void    set(const c8* name, const u32 val);
        void    set(const c8* name, const s32 val);
        void    set(const c8* name, const f32 val);
        void    set(const c8* name, const bool val);
        void    set(const c8* name, const json& val);
                
    private:
        json_object* m_internal_object;
        void copy( json* dst, const json& other );
    };
}

#endif
