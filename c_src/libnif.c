#include <stdbool.h>
#include <stdint.h>
#include <math.h>
#include <erl_nif.h>

#define PI 3.1415926535897932384626433832795028841971

void sin16_mac_horner(uint64_t size, __fp16 *array)
{
    __fp16 *p = array;
    for(uint64_t i = 0; i < size; i++) {
        __fp16 x = *p;
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
        *p = y;
        p++;
    }
}

void sin16_mac(uint64_t size, __fp16 *array)
{
    __fp16 *p = array;
    for(uint64_t i = 0; i < size; i++) {
        __fp16 x = *p;
        x = x - floor(x);
        if(x >= 0.5) {
            x = 0.5 - x;
        }
        if(x >= 0.25) {
            x = 0.5 - x;
        }
        x *= 2 * PI;
        *p = x 
            - (x * x * x) / 6 
            + (x * x * x * x * x) / 120 
            - (x * x * x * x * x * x * x) / 5040;
        p++;
    }
}

void sin32(uint64_t size, float *array)
{
    for(uint64_t i = 0; i < size; i++) {
        array[i] = sin(array[i] * 2 * PI);
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
    ErlNifBinary binary_data;
    if(__builtin_expect(!enif_inspect_binary(env, binary_term, &binary_data), false)) {
        return enif_make_badarg(env);
    }

    __fp16 *array = (__fp16 *)binary_data.data;
    sin16_mac_horner(vec_size, array);

    return enif_make_binary(env, &binary_data);
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
    ErlNifBinary binary_data;
    if(__builtin_expect(!enif_inspect_binary(env, binary_term, &binary_data), false)) {
        return enif_make_badarg(env);
    }

    __fp16 *array = (__fp16 *)binary_data.data;
    sin16_mac(vec_size, array);

    return enif_make_binary(env, &binary_data);
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
    ErlNifBinary binary_data;
    if(__builtin_expect(!enif_inspect_binary(env, binary_term, &binary_data), false)) {
        return enif_make_badarg(env);
    }

    float *array = (float *)binary_data.data;
    sin32(vec_size, array);

    return enif_make_binary(env, &binary_data);
}

static ErlNifFunc nif_funcs[] = 
{
    {"sin32_nif", 2, sin32_nif},
    {"sin16_mac_nif", 2, sin16_mac_nif},
    {"sin16_mac_horner_nif", 2, sin16_mac_horner_nif}
};

ERL_NIF_INIT(Elixir.NxNif, nif_funcs, NULL, NULL, NULL, NULL)
