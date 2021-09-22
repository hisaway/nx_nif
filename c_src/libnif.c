#include <stdbool.h>
#include <stdint.h>
#include <math.h>
#include <erl_nif.h>

void sin32(uint64_t size, float *array)
{
    for(uint64_t i = 0; i < size; i++) {
        array[i] = sin(array[i]);
    }
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
    if(__builtin_expect(!enif_term_to_binary(env, binary_term, &binary_data), false)) {
        return enif_make_badarg(env);
    }

    float *array = (float *)(&binary_data.data[6]);
    sin32(vec_size, array);

    if(__builtin_expect(enif_binary_to_term(env, binary_data.data, binary_data.size, &binary_term, 0) == 0, false)) {
        return enif_make_badarg(env);
    }
    return binary_term;
}

static ErlNifFunc nif_funcs[] = 
{
    {"sin32_nif", 2, sin32_nif}
};

ERL_NIF_INIT(Elixir.NxNif, nif_funcs, NULL, NULL, NULL, NULL)
