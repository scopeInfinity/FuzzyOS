## How to contribute

#### Why was FuzzyOS created?

* It's primarily a Hobby Project with aim of learning some trivial challenges during OS development.

#### Did you find a bug?

* I won't be surprised. Feel free to [create a GitHub issue](https://github.com/scopeInfinity/FuzzyOS/issues/new) and raise a pull request to submit a patch.

#### Did you write a patch that fixes a bug?

* Please ensure code is formatted as per clang-format `{BasedOnStyle: llvm, IndentWidth: 4}` style.

    ```
    find . -iname *.h -o -iname *.c -o -iname *.cpp -o -iname *.hpp -o -iname *.tcc | xargs clang-format --style="{BasedOnStyle: llvm, IndentWidth: 4}" -i 
    ```

* Open a new GitHub pull request with the patch.

* Ensure the PR description clearly describes the problem and solution. Include the relevant issue number if applicable.

#### **Do you intend to add a new feature or change an existing one?**

* Suggest the feature or change by opening a [new GitHub issue](https://github.com/scopeInfinity/FuzzyOS/issues/new). After discussing it with the maintainer one can proceed with the patch.

Thanks!
