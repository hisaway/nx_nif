Application.put_env(:exla, :clients,
  default: [platform: :host],
  cuda: [platform: :cuda]
)

defmodule Sin do
  import Nx.Defn

  @defn_compiler EXLA
  defn host(n), do: Nx.sin(n)

  @defn_compiler {EXLA, client: :cuda}
  defn cuda(n), do: Nx.sin(n)

  @defn_compiler {EXLA, client: :cuda, run_options: [keep_on_device: true]}
  defn cuda_keep(n), do: Nx.sin(n)
end

input_f32 = Nx.iota({0x100}, type: {:f, 32}) |> Nx.multiply(0.1)
input_f16 = Nx.iota({0x100}, type: {:f, 16}) |> Nx.multiply(0.1)
input_f32_pi = Nx.multiply(input_f32, 2 * :math.pi)

benches = %{
  "NIF 32" => fn -> NxNif.sin32(input_f32) end,
  "NIF 16 mac" => fn -> NxNif.sin16_mac(input_f16) end,
  "Nx" => fn -> Nx.sin(input_f32_pi) end,
  "xla jit-cpu" => fn -> Sin.host(input_f32_pi) end
}

benches =
  if System.get_env("EXLA_TARGET") == "cuda" do
    dt32 = Nx.backend_transfer(input_f32_pi, {EXLA.DeviceBackend, client: :cuda})

    Map.merge(benches, %{
      "xla jit-gpu" => fn -> Sin.cuda(dt32) end,
      "xla jit-gpu keep" => {fn -> Sin.cuda_keep(dt32) end, after_each: &Nx.backend_deallocate/1}
    })
  else
    benches
  end

Benchee.run(
  benches,
  time: 10,
  memory_time: 2
) \
|> then(fn _ -> :ok end)
