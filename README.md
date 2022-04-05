# CP 386 Assignment 4 - DUE DATE: APR 4TH 11:59 PM


**Contributors**
- Carson Aziz - 180801040 - aziz0140 - aziz0140@mylaurier.ca
  - github account that made commits is "carsonaziz"
- Raiyan Jugbhery - 180686050 - jugb6050 - jugb6050@mylaurier.ca

-----------------------------------------

Licensing: Developed by Raiyan Jugbhery and Carson Aziz. Only for view/use by IA/Professor for marking purposes at Wilfrid Laurier University

-----------------------------------
**Individual Contribution Split:**

Question 1 - Raiyan Jugbhery

Question 2 - Carson Aziz

---------------------------------
## Question 1
**Question 1 Features:** Raiyan Jugbhery

Extra testing added for if incorrect values are requested or released:
  
  -Checks to see if user is requesting to release resources that are not allocated
    
  -Check to see if resources requested is less than available, and also fail safes the user from requesting greater than what their
  maximum needed resource is.
  
-Fail safes any incorrect commands that are not outlined within the assignment

-Error checks if file exists or not

-Checks to see if customer number is valid, this is done by getting number of customers from sample file, then checking to see if user input matches a number that is between 0 and max customer number

-------------------------------------------

Test Cases for Question 1: Raiyan Jugbhery

- Incorrect Input for command

- Exit Command works correctly

- Status Command works Correctly

- Run Command works Correctly

- RQ Command works correctly

- RL Command works correctly

- Correct Input/Output following Assignment Instructions

-----------------------------------------------

**Screenshots of Testing for Question 1:**

  _- Regular Testing for Correct Output from given assignment inputs:_

<img width="1168" alt="Screen Shot 2022-04-04 at 6 17 05 AM" src="https://user-images.githubusercontent.com/60024880/161523998-33a4b849-acba-463b-b553-f95b72fde6c5.png">
<img width="1173" alt="Screen Shot 2022-04-04 at 6 18 10 AM" src="https://user-images.githubusercontent.com/60024880/161524154-38e8b93f-dff4-4966-a4f1-7b33589c270d.png">
<img width="1171" alt="Screen Shot 2022-04-04 at 6 18 36 AM" src="https://user-images.githubusercontent.com/60024880/161524223-c39cd151-08a0-4013-a0a6-380c2d4cb50b.png">
<img width="1170" alt="Screen Shot 2022-04-04 at 6 18 51 AM" src="https://user-images.githubusercontent.com/60024880/161524256-075af295-6f0a-4b69-9692-ce041b779d7d.png">
<img width="1169" alt="Screen Shot 2022-04-04 at 6 19 11 AM" src="https://user-images.githubusercontent.com/60024880/161524321-54a19b31-922c-4134-8627-6518d984722a.png">

  _- Testing of Incorrect Input:_  
   <img width="772" alt="Screen Shot 2022-04-04 at 6 24 40 AM" src="https://user-images.githubusercontent.com/60024880/161525167-6bcf5634-a2bc-4f9f-95eb-dc924a18bebe.png">


---------------------------------
## Question 2
**Question 2 Features:** Carson Aziz

- Process can request a chunk of memory

- Memory is allocated in chunks of 10 bytes to prevent internal memory fragmentation

- Process can be released and it's memory will be free for other processes to use

- The status of the memory can be viewed, showing allocated memory and the processes using it, and free memory and the holes that represent it

-------------------------------------------

Test Cases for Question 2: Carson Aziz

- Releasing process which doesn't have memory allocated for it

- Exit Command works correctly

- Status Command works Correctly

- RQ Command works correctly

- RL Command works correctly

- Correct Input/Output following Assignment Instructions

-----------------------------------------------

**Screenshots of Testing for Question 2:**

  _- Testing done using Makefile using given assignment inputs as default:_


-----------------------------------------------

## Makefile

**Makefile Commands**
```
make all
make clean
make runq1
make runq2
```

- `make all` command will compile both files
- `make clean` will remove binary and object files
- `make runq1` will run Question 1 with the default inputs
- `make runq2` will run Question 2 with the default inputs
-----------------------------------------------
## License
**Apache License 2.0**\
Permisions
- Commercial Use
- Modification
- Distribution
- Patent Use
- Private Use

Limitations
- Trademark Use
- Liability
- Warranty
-----------------------------------------------
