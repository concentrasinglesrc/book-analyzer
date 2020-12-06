# Book Analyzer

## Functional Requirements 

[ ] Inputs 
    [x] Read from stdin log files
    [x] Read line by line from stdin
    [x] Read arguments:
        - target-size: long
    [x] Parse command via a controller.
    [x] Deserialize Order command by:
        - order type: Reduce|Add
            - Reduce:
                - timestamp: long
                - order-id: string
                - size (shares): long
            - Add:
                - timestamp: long
                - order-id: string
                - size (shares): long
                - price: float
                - side: Buy | Sell

[ ] Outputs
    [x] Print to stderr warning and errors
    [x] Write actions to stdout
        - Serialize Action by:
            - timestamp: long
            - action type: Buy | Sell
            - total: float | NA
[x] Validation
    [x] terminate with error when 1st arg is not present
    [x] print warning as to stderr when input message is invalid, skip message
    [x] field parsing errors
        [x] unsupported side
        [x] negative timestamp and above 24 hrs
        [x] unsupported order type
        [x] nonnumeric strings in numeric fields
        [x] missing fields for reduce order
        [x] missing fields for add order
[ ] Models
    [x] model order: bidorder | askorder
    [x] model side: buy | sell
    [x] model commands: reduce|add
    [x] model params: addorder |reduceorder | action
[ ] Business Logic
    [x] book starts with no orders
    [x] expenses starts with NA
    [x] income starts with
    [x] on add to order bid, save order id to book
    [ ] on add to order sell, save order id to book
    [ ] calculate expenses when total buy shares is greater than or equal to target size
    [ ] calculate income when total sell shares are greater than or equal to target size 
    [ ] on reduce order, update order id to book
    [ ] expenses are NA when reduce order reduces total buy shares under target size
    [ ] income is NA when reduce order reduces total sell shares under target size
    [ ] skip printing to stdout when add to order is processed but total shares are less than target size
    [ ] calculate expenses with the lowest bid price first
    [ ] calculate income with the highest ask price first

## Non-Functional Requirements

[ ] capacity
    [ ] ~able to read x lines : approx x textfile~
    [ ] what is the performance? records per/second
[ ] development
    [ ] cli end to end test
    [ ] unit-testing (optional with gtest)
    [ ] autotools setup
    [ ] ? boost
[ ] docs    
    [ ] readme setup
    [ ] quick start
    [ ] dependecies