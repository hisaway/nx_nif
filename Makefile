.phony: all clean

ifeq ($(ERL_EI_INCLUDE_DIR),)
	ERLANG_PATH = $(shell elixir --eval ':code.root_dir |> to_string() |> IO.puts')
	ifeq ($(ERLANG_PATH),)
		$(error Could not find the Elixir installation. Check to see that 'elixir')
	endif
ERL_EI_INCLUDE_DIR = $(ERLANG_PATH)/usr/include
ERL_EI_LIB_DIR = $(ERLANG_PATH)/usr/lib
endif

ERL_CFLAGS ?= -I$(ERL_EI_INCLUDE_DIR)
ERL_LDFLAGS ?= -L$(ERL_EI_LIBDIR)

CFLAGS += -I/usr/local/include -I/usr/include -I/usr/local/lib
LDFLAGS += -L/usr/lib -shared
CFLAGS += -std=c11 -O3 -Wall -Wextra -Wno-unused-function -Wno-unused-parameter -Wno-missing-field-initializers

ifeq ($(CROSSCOMPILE),)
	ifneq ($(OS),Windows_NT)
		CFLAGS += -fPIC
		LDFLAGS += -fPIC
		ifeq ($(shell uname),Darwin)
			LDFLAGS += -undefined dynamic_lookup
		endif	
	endif
endif

NIF = priv/libnif.so

all: priv $(NIF)

priv:
	mkdir -p priv

$(NIF): c_src/libnif.c
	$(CC) $(ERL_CFLAGS) $(CFLAGS) $(ERL_LDFLAGS) $(LDFLAGS) \
		-o $@ $<

clean:
	$(RM) $(NIF)
