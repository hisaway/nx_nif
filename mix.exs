defmodule NxNif.MixProject do
  use Mix.Project

  def project do
    [
      app: :nx_nif,
      version: "0.1.3",
      elixir: "~> 1.12",
      start_permanent: Mix.env() == :prod,
      deps: deps(),
      compilers: [:elixir_make | Mix.compilers()],
      make_targets: ["all"],
      make_clean: ["clean"],
      build_embedded: true
    ]
  end

  # Run "mix help compile.app" to learn about applications.
  def application do
    [
      extra_applications: [:logger]
    ]
  end

  # Run "mix help deps" to learn about dependencies.
  defp deps do
    [
      {:nx, "~> 0.1.0-dev", github: "elixir-nx/nx", branch: "main", sparse: "nx"},
      {:elixir_make, "~> 0.4", runtime: false},
      {:benchee, "~> 1.0", only: :dev}
    ]
  end
end
