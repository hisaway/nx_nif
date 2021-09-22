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

  def test(), do: raise("NIF test/0 not implemented")

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
