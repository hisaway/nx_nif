input_f32 = Nx.iota({0x100}, type: {:f, 32}) |> Nx.multiply(0.1)
input_f16 = Nx.iota({0x100}, type: {:f, 16}) |> Nx.multiply(0.1)
input_f32_pi = Nx.multiply(input_f32, 2 * :math.pi)

Benchee.run(
  %{
    "NIF 32" => fn -> NxNif.sin32(input_f32) end,
    "NIF 16 mac" => fn -> NxNif.sin16_mac(input_f16) end,
    "Nx" => fn -> Nx.sin(input_f32_pi) end,
  }
) \
|> then(fn _ -> :ok end)
