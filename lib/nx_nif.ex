defmodule NxNif do
  require Logger

  @moduledoc """
  Documentation for `NxNif`.
  """

  @on_load :load_nif

  def load_nif do
    nif_file = 'priv/libnif'

    case :erlang.load_nif(nif_file, 0) do
      :ok -> :ok
      {:error, {:reload, _}} -> :ok
      {:error, reason} -> Logger.error("Failed to load NIF: #{inspect(reason)}")
    end
  end

  def sin32(x) when is_struct(x, Nx.Tensor) do
    if Nx.type(x) == {:f, 32} do
      x
    else
      Nx.as_type(x, {:f, 32})
    end
    |> sin32_sub()
  end

  def sin32(x) when is_number(x) do
    sin32(Nx.tensor([x]))
  end

  defp sin32_sub(t) do
    %{
      t
      | data: %{
          t.data
          | state: sin32_nif(Nx.size(t), t.data.state)
        }
    }
  end

  def sin32_nif(_size, _x), do: raise("NIF sin32_nif/2 not implemented")

  @doc """
  Hello world.

  ## Examples

      iex> NxNif.hello()
      :world

  """
  def hello do
    :world
  end
end
