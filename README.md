# Algebra Solver API

This is a algebra solver API. Which accepts `Algebraic Expressions` and sends back a step by step simplification of Algebraic Expression

#### `POST` /

## Parameters to execute API calls

| Parameter  | Description                                                             |
| ---------- | ----------------------------------------------------------------------- |
| expression | Should contain the algebraic expression, this is going to be simplified |
| variables  | Contains a list of substitute values for variables in the expression    |

## How to contruct an `expression`

Every algebraic expression has `numbers`, `variables` and `mathematical operators`.

#### Mathematical Operators :

| value | Description                          |
| ----- | ------------------------------------ |
| +     | This is the addition operator        |
| -     | This is the subtraction operator     |
| \*    | This is the multiplication operator  |
| /     | This is the division operator        |
| ^     | This operator is used to raise power |

#### Examples of Mathematical expressions :

| expression                    | Equivalent mathematical form                        |
| ----------------------------- | --------------------------------------------------- |
| `2x + y`                      | _2x + y_                                            |
| `(a + b)^2`                   | _( a + b )<sup> 2</sup>_                            |
| ` -200 +- x^2 ( 2.5x + 3y^z)` | _-200 +- x<sup> 2 </sup> (2.5x_ + 3y<sup> z </sup>) |

## How to pass `variables` for subtitution during simplification

`variables` field must contain key value pairs of `variable_name(type string) : variable_value(type number)`

### Example Request data

```json
{
  "expression": "x^2 + y",
  "variables": { "x": 20, "y": 5.5 }]
}
```

Now when we make a call to this API with this data as JSON body.
A response is received with step by step substitution and simplification.

### Response

```json
{ "steps": ["20^2 + 5.5", "400 + 5.5", "405.5"] }
```

**Please Note:** The values of variables are not a required field.

## Request data without `variables` field

```json
{ "expression": "(a + b)^2" }
```

### Response

```json
{ "steps": ["(a + b)^2", "a^2 + 2ab + b^2"] }
```

## NodeJS example to execute API call

```javascript
const axios = require("axios").default;

const options = {
  method: "GET",
  url: "https://algebraapi.onrender.com/",
  data: { expression: "(a + b)^2", variables: { x: 2 } },
};

axios
  .request(options)
  .then(function (response) {
    console.log(response.data);
  })
  .catch(function (error) {
    console.error(error);
  });
```

## Response

```javascript
{
  steps: ["(2 + b)^2", "4 + 4b + b^2"];
}
```

The Algebra Solver has been tested on various types of algebraic expression. But, it is not guarenteed that the response is always mathematically correct.

**Please, inform me with details in which the API responds with wrong data.**
