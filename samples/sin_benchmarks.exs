input_f32 = Nx.iota({0x1000000}, type: {:f, 32})

Benchee.run(
  %{
    "NIF" => fn -> NxNif.sin32(input_f32) end,
    "Nx" => fn -> Nx.sin(input_f32) end,
  }
) \
|> then(fn _ -> :ok end)
