import os
import readline
import numpy as np
import matplotlib.pyplot as plt

def plot_price_from_csv(file_path):
    prices = np.genfromtxt(
        file_path,
        delimiter=",",
        usecols=0,
        invalid_raise=False
    )

    # Remove NaNs caused by headers or bad lines
    prices = prices[~np.isnan(prices)]
    prices = prices [:1000]
    plt.figure()
    plt.plot(prices )
    plt.xlabel("Time (index)")
    plt.ylabel("Price")
    plt.title("Price Evolution")
    plt.show()


def completer(text, state):
    raw = os.path.expanduser(text)

    # Preserve trailing slash info
    has_trailing_sep = raw.endswith(os.sep)

    # Normalize path (cleans ./ ./ ../ etc)
    norm = os.path.normpath(raw)

    if has_trailing_sep:
        norm += os.sep

    dirname, partial = os.path.split(norm)

    if dirname == "":
        dirname = "."

    try:
        entries = os.listdir(dirname)
    except OSError:
        return None

    matches = []
    for entry in entries:
        if entry.startswith(partial):
            full = os.path.join(dirname, entry)
            if os.path.isdir(full):
                matches.append(full + os.sep)
            else:
                matches.append(full)

    try:
        return matches[state]
    except IndexError:
        return None

# readline.set_completer(completer)
# readline.parse_and_bind("tab: complete")

if __name__ == "__main__":
    print("Where is the data we are going to use? [Input File Path]")
    x = input()
    if x == "":
        x = "../Mean-Reversion/logs/stable-mean-reversion-log-1.csv"
    plot_price_from_csv(x)
