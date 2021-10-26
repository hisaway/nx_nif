#include <stdbool.h>
#include <stdint.h>
#include <math.h>
#include <erl_nif.h>

#define PI 3.1415926535897932384626433832795028841971

void sin16_mac_horner(uint64_t size, __fp16 *in, __fp16 *out)
{
    __fp16 *pin = in;
    __fp16 *pout = out;
    for(uint64_t i = 0; i < size; i++) {
        __fp16 x = *pin++;
        x = x - floor(x);
        if(x >= 0.5) {
            x = 0.5 - x;
        }
        if(x >= 0.25) {
            x = 0.5 - x;
        }
        x *= 2 * PI;
        __fp16 xx = x * x;
        __fp16 y = -1.0 / 5040;
        y *= xx;
        y += 1.0 / 120;
        y *= xx;
        y -= 1.0 / 6;
        y *= xx;
        y += 1;
        y *= x;
        *pout++ = y;
    }
}

void sin16_mac(uint64_t size, __fp16 *in, __fp16 *out)
{
    __fp16 *pin = in;
    __fp16 *pout = out;
    for(uint64_t i = 0; i < size; i++) {
        __fp16 x = *pin++;
        x = x - floor(x);
        if(x >= 0.5) {
            x = 0.5 - x;
        }
        if(x >= 0.25) {
            x = 0.5 - x;
        }
        x *= 2 * PI;
        *pout++ = x 
            - (x * x * x) / 6 
            + (x * x * x * x * x) / 120 
            - (x * x * x * x * x * x * x) / 5040;
    }
}

void sin32_mac_horner(uint64_t size, float *in, float *out)
{
    float *pin = in;
    float *pout = out;
    for(uint64_t i = 0; i < size; i++) {
        float x = *pin++;
        x = x - floor(x);
        if(x >= 0.5) {
            x = 0.5 - x;
        }
        if(x >= 0.25) {
            x = 0.5 - x;
        }
        x *= 2 * PI;
        float xx = x * x;
        float y = 1.0 / 6227020800;
        y *= xx;
        y -= 1.0 / 39916800;
        y *= xx;
        y += 1.0 / 362800;
        y *= xx;
        y -= 1.0 / 5040;
        y *= xx;
        y += 1.0 / 120;
        y *= xx;
        y -= 1.0 / 6;
        y *= xx;
        y += 1;
        y *= x;
        *pout++ = y;
    }
}

void sin32_mac(uint64_t size, float *in, float *out)
{
    float *pin = in;
    float *pout = out;
    for(uint64_t i = 0; i < size; i++) {
        float x = *pin++;
        x = x - floor(x);
        if(x >= 0.5) {
            x = 0.5 - x;
        }
        if(x >= 0.25) {
            x = 0.5 - x;
        }
        x *= 2 * PI;
        *pout++ = x 
            - (x * x * x) / 6 
            + (x * x * x * x * x) / 120 
            - (x * x * x * x * x * x * x) / 5040
            + (x * x * x * x * x * x * x * x * x) / 362880 
            - (x * x * x * x * x * x * x * x * x * x * x) / 39916800 
            + (x * x * x * x * x * x * x * x * x * x * x * x * x) / 6227020800 
            - (x * x * x * x * x * x * x * x * x * x * x * x * x * x * x) / 1307674400000;
    }
}

void sin32(uint64_t size, float *in, float *out)
{
    for(uint64_t i = 0; i < size; i++) {
        out[i] = sin(in[i] * 2 * PI);
    }
}

static ERL_NIF_TERM sin16_mac_horner_nif(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[])
{
    if(__builtin_expect(argc != 2, false)) {
        return enif_make_badarg(env);
    }
    ErlNifUInt64 vec_size;
    if(__builtin_expect(!enif_get_uint64(env, argv[0], &vec_size), false)) {
        return enif_make_badarg(env);
    }

    ERL_NIF_TERM binary_term = argv[1];
    ErlNifBinary in_data;
    if(__builtin_expect(!enif_inspect_binary(env, binary_term, &in_data), false)) {
        return enif_make_badarg(env);
    }

    __fp16 *in = (__fp16 *)in_data.data;
    ErlNifBinary out_data;
    if(__builtin_expect(!enif_alloc_binary(vec_size * sizeof(__fp16), &out_data), false)) {
        return enif_make_badarg(env);
    }
    __fp16 *out = (__fp16 *)out_data.data;

    sin16_mac_horner(vec_size, in, out);

    return enif_make_binary(env, &out_data);
}

static ERL_NIF_TERM sin16_mac_nif(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[])
{
    if(__builtin_expect(argc != 2, false)) {
        return enif_make_badarg(env);
    }
    ErlNifUInt64 vec_size;
    if(__builtin_expect(!enif_get_uint64(env, argv[0], &vec_size), false)) {
        return enif_make_badarg(env);
    }

    ERL_NIF_TERM binary_term = argv[1];
    ErlNifBinary in_data;
    if(__builtin_expect(!enif_inspect_binary(env, binary_term, &in_data), false)) {
        return enif_make_badarg(env);
    }

    __fp16 *in = (__fp16 *)in_data.data;
    ErlNifBinary out_data;
    if(__builtin_expect(!enif_alloc_binary(vec_size * sizeof(__fp16), &out_data), false)) {
        return enif_make_badarg(env);
    }
    __fp16 *out = (__fp16 *)out_data.data;

    sin16_mac(vec_size, in, out);

    return enif_make_binary(env, &out_data);
}

static ERL_NIF_TERM sin32_mac_horner_nif(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[])
{
    if(__builtin_expect(argc != 2, false)) {
        return enif_make_badarg(env);
    }
    ErlNifUInt64 vec_size;
    if(__builtin_expect(!enif_get_uint64(env, argv[0], &vec_size), false)) {
        return enif_make_badarg(env);
    }

    ERL_NIF_TERM binary_term = argv[1];
    ErlNifBinary in_data;
    if(__builtin_expect(!enif_inspect_binary(env, binary_term, &in_data), false)) {
        return enif_make_badarg(env);
    }

    float *in = (float *)in_data.data;
    ErlNifBinary out_data;
    if(__builtin_expect(!enif_alloc_binary(vec_size * sizeof(float), &out_data), false)) {
        return enif_make_badarg(env);
    }
    float *out = (float *)out_data.data;

    sin32_mac_horner(vec_size, in, out);

    return enif_make_binary(env, &out_data);
}

static ERL_NIF_TERM sin32_mac_nif(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[])
{
    if(__builtin_expect(argc != 2, false)) {
        return enif_make_badarg(env);
    }
    ErlNifUInt64 vec_size;
    if(__builtin_expect(!enif_get_uint64(env, argv[0], &vec_size), false)) {
        return enif_make_badarg(env);
    }

    ERL_NIF_TERM binary_term = argv[1];
    ErlNifBinary in_data;
    if(__builtin_expect(!enif_inspect_binary(env, binary_term, &in_data), false)) {
        return enif_make_badarg(env);
    }

    float *in = (float *)in_data.data;
    ErlNifBinary out_data;
    if(__builtin_expect(!enif_alloc_binary(vec_size * sizeof(float), &out_data), false)) {
        return enif_make_badarg(env);
    }
    float *out = (float *)out_data.data;

    sin32_mac(vec_size, in, out);

    return enif_make_binary(env, &out_data);
}

static ERL_NIF_TERM sin32_nif(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[])
{
    if(__builtin_expect(argc != 2, false)) {
        return enif_make_badarg(env);
    }
    ErlNifUInt64 vec_size;
    if(__builtin_expect(!enif_get_uint64(env, argv[0], &vec_size), false)) {
        return enif_make_badarg(env);
    }

    ERL_NIF_TERM binary_term = argv[1];
    ErlNifBinary in_data;
    if(__builtin_expect(!enif_inspect_binary(env, binary_term, &in_data), false)) {
        return enif_make_badarg(env);
    }

    float *in = (float *)in_data.data;
    ErlNifBinary out_data;
    if(__builtin_expect(!enif_alloc_binary(vec_size * sizeof(float), &out_data), false)) {
        return enif_make_badarg(env);
    }
    float *out = (float *)out_data.data;

    sin32(vec_size, in, out);

    return enif_make_binary(env, &out_data);
}

static ErlNifFunc nif_funcs[] = 
{
    {"sin32_nif", 2, sin32_nif},
    {"sin32_mac_nif", 2, sin32_mac_nif},
    {"sin32_mac_horner_nif", 2, sin32_mac_horner_nif},
    {"sin16_mac_nif", 2, sin16_mac_nif},
    {"sin16_mac_horner_nif", 2, sin16_mac_horner_nif},
};

ERL_NIF_INIT(Elixir.NxNif, nif_funcs, NULL, NULL, NULL, NULL)
