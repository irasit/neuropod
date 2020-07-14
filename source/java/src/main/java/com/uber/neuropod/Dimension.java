package com.uber.neuropod;

/**
 * The type Dimension.
 */
public class Dimension {
    /**
     * The Value. -1 means non/null, any value is OK
     * -2 menas this is a symbol
     */
    long value;
    /**
     * The Symbol.
     */
    String symbol;

    /**
     * Instantiates a new Dimension by given value
     *
     * @param value the value
     */
    public Dimension(long value) {
        this.value = value;
    }

    /**
     * Instantiates a new Dimension by given symbol
     *
     * @param symbol the symbol
     */
    public Dimension(String symbol) {
        this.symbol = symbol;
        this.value = -2;
    }

    /**
     * Gets value.
     *
     * @return the value
     */
    public long getValue() {
        return value;
    }

    /**
     * Gets symbol.
     *
     * @return the symbol
     */
    public String getSymbol() {
        return symbol;
    }

    @Override
    public String toString() {
        if (value >= 0) {
            return String.valueOf(value);
        } else if (value == -1){
            return "None";
        } else if (value == -2) {
            return symbol;
        }
        return "Not Set!";
    }
}
