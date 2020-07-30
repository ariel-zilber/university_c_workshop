/**
* @file manageStudents.c
* @author  Ariel Zilbershtein
* @version 1.0
* @date 11 Nov 2019
*
* @brief Solutions to ex1.
*
*
* @section DESCRIPTION
* The programs  performs actions on provided list of students.
*/

// ------------------------------ includes ------------------------------
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <math.h>
// -------------------------- const definitions -------------------------

/**
* @def EXIT_SUCCESS 0
* @brief Defines program exit as success.
*/
#define EXIT_SUCCESS 0

/**
* @def EXIT_FAILURE 1
* @brief Defines program exit as failue.
*/
#define EXIT_FAILURE 1

/**
* @def EXIT_COMMAND "q\r\n"
* @brief Command used to exit the program.
*/
#define EXIT_COMMAND "q\r\n"

/**
* @def ID_FIELD_LENGTH  10
* @brief The max length the id field should have
*/
#define ID_FIELD_LENGTH  10

/**
* @def DEFAULT_FIELD_LENGTH  40
* @brief The max length all fields(expect id) should have
*/
#define DEFAULT_FIELD_LENGTH  40

/**
* @def MIN_GRADE 100
* @brief The minimal grade value
*/
#define MIN_GRADE  0

/**
* @def MAX_GRADE 100
* @brief The maximal grade value
*/
#define MAX_GRADE 100

/**
* @def MIN_AGE 18
* @brief The minimal age possible for a student
*/
#define MIN_AGE 18

/**
* @def MAX_AGE 120
* @brief The max age possible for a student
*/
#define MAX_AGE 120

/**
* @def MIN_ID "0999999999"
* @brief The minimal id possible for a student
*/
#define MIN_ID "1000000000"

#define NO_STUDENTS_MESSAGE "ERROR: There are not any students.Program will now exit\n"

/**
* @def EPSILON 0.00001
* @brief The minial diffence float types can have to be considered equal
*/
#define EPSILON 0.00001

/**
* @def MAX_INPUT_ROWS 5000
* @brief The maximal input a user can enter
*/
#define MAX_INPUT_ROWS 5000

/**
* @def MAX_STUDENT_ENTRIES 5000
* @brief The maximal amount of student entries
*/
#define MAX_STUDENT_ENTRIES 5000

/**
* @def LINE_MAX_LENGTH 150
* @brief Defines the maximal length that an user can provide as input.
*/
#define LINE_MAX_LENGTH 150


/**
* @def EXPECTED_TOTAL_FIELDS 6
* @brief The expected number of field should be given when entering user details
*/
#define EXPECTED_TOTAL_FIELDS 6

/**
* @def ENTER_INFO_MESSAGE "Enter student info. To exit press q, then enter\n"
* @brief The message appearing when enter input
*/
#define ENTER_INFO_MESSAGE "Enter student info. To exit press q, then enter\n"

/**
* @def ERROR_PARAMETERS_NUM "parameters were not provided!!"
* @brief The message error displayed when an invalid parameter count was provided
*/
#define ERROR_PARAMETERS_NUM "ERROR: parameters were not provided!!\n"

/**
* @def ERROR_FIELD_NUM "ERROR: Invalid number of parameters was given!"
* @brief The message error displayed when incorrect number of fields were provided by the user
*/
#define ERROR_FIELD_NUM "ERROR: Invalid number of parameters was given!\n"

/**
* @def ERROR_PARAMETERS_WRONG_TYPE "a wrong parameter was given"
* @brief Message displayed when an invalid parameter was given
*/
#define ERROR_PARAMETERS_WRONG_TYPE "USAGE: a wrong parameter was given\n"

/**
* @def ID_TYPE_ERROR_MSG "ERROR: ID can only contain alphabetic characters or '-' "
* @brief Message displayed when invalid parameters types were given to the id field
*/
#define ID_TYPE_ERROR_MSG "ERROR: ID can only contain 10 integers between 0 to 9\n"

/**
* @def ID_VALUE_ERROR_MSG "ERROR: ID must be atleast 10000000000\n"
* @brief Message displayed when the id given was below the value range
*/
#define ID_VALUE_ERROR_MSG "ERROR: ID must be atleast 10000000000\n"


/**
* @def NAME_TYPE_ERROR_MSG "ERROR: Name can only contain alphabetic characters or '-'\n"
* @brief Message displayed when invalid parameters types were given to the name field
*/
#define NAME_TYPE_ERROR_MSG "ERROR: Name can only contain alphabetic characters or \'-\'\n"

/**
* @def NAME_TYPE_ERROR_MSG "ERROR: Name can only contain alphabetic characters or '-'\n"
* @brief Message displayed when invalid parameters types were given to the age field
*/
#define AGE_TYPE_ERROR_MSG "ERROR: Age must represent a number\n"

/**
* @def AGE_VALUE_ERROR_MSG "ERROR: age can only contain integers between 18 to 120\n"
* @brief Message displayed an age was given outside the expected range
*/
#define AGE_VALUE_ERROR_MSG "ERROR: age can only contain integers between 18 to 120\n"

/**
* @def GRADE_TYPE_ERROR_MSG "ERROR: Grade must represent a number\n"
* @brief Message displayed when invalid parameters types were given to the grade field
*/

#define GRADE_TYPE_ERROR_MSG "ERROR: Grade must represent a number\n"
/**
* @def AGE_VALUE_ERROR_MSG "ERROR: grade can only contain integers between 18 to 120\n"
* @brief Message displayed a grade was given outside the expected range
*/
#define GRADE_VALUE_ERROR_MSG "ERROR: grade can only contain integers between 18 to 120\n"

/**
* @def CITY_TYPE_ERROR_MSG "ERROR: City can only contain alphabetic characters or '-'\n"
* @brief Message displayed when invalid parameters types were given to the city field
*/
#define CITY_TYPE_ERROR_MSG "ERROR: City can only contain alphabetic characters or \'-\'\n"

/**
* @def CITY_TYPE_ERROR_MSG "ERROR: Country can only contain alphabetic characters or '-'\n"
* @brief Message displayed when invalid parameters types were given to the country field
*/
#define COUNTRY_TYPE_ERROR_MSG "ERROR: Country can only contain alphabetic characters or \'-\'\n"


/**
* @def COMMAND_BEST "best"
* @brief Represents the best action as was described in the exercise description
*/
#define COMMAND_BEST "best"

/**
* @def COMMAND_QUICK "quick"
* @brief Represents the quick action as was described in the exercise description
*/
#define COMMAND_QUICK "quick"

/**
* @def COMMAND_MERGE "merge"
* @brief Represents the merge action as was described in the exercise description
*/
#define COMMAND_MERGE "merge"
/**
* @def MESSAGE_FOUND_BEST "best student is:\n"
* @brief Message displayed after finding the best student
*/
#define MESSAGE_FOUND_BEST "best student info is: "

// ------------------------------ Structures -----------------------------

/**
 * @brief represents an entry data for a student
 * */
typedef struct Student
{
    char id[ID_FIELD_LENGTH + 1];  /** Represents the id of the student */
    char name[DEFAULT_FIELD_LENGTH + 1];  /** Represents  the full name of the student */
    int grade; /** Represents the grade the student has */
    int age;   /** Represents the student's age */
    char country[DEFAULT_FIELD_LENGTH + 1]; /** Represent the student's country */
    char city[DEFAULT_FIELD_LENGTH + 1];   /** Represents the student's city */

} Student;


/**  Represents the  data of the program**/
Student studentEntries[MAX_STUDENT_ENTRIES] = {};

/** represents the total amount of entered students data **/
int totalStudentsEntries = 0;

/** Helper arrays used in the mergesort algortihrrm used in this program**/
Student leftMergeSortArray[MAX_STUDENT_ENTRIES] = {};
Student rightMergeSortArray[MAX_STUDENT_ENTRIES] = {};

// ------------------------------ function prototype --------------------


void actionBest();

void actionMerge();

void actionQuick();

void getUserEntriesInput();

void addStudentEntry(char *);

int compareStudentsQuality(Student firstStudent, Student secondsStudent);

void printStudentInfo(Student student);

void printStudentsInfo();

void printBestStudent();

void merge(int l, int m, int r);

void mergeSort(int l, int r);

void swapStudents(Student *, Student *);

int partition(int left, int right);

void quickSort(int left, int right);

int getTabCount(char *line);

bool isNumber(char *input);

int getNumberFromString(char *str);

bool isValidWord(char *input);

bool validateStudentInputLine(char *line, int lineCount);

bool isInRange(int number, int lowerLimit, int upperLimit);
// ------------------------------ functions -----------------------------


/**
* @brief The main function.Runs the student management program
 * @param argv takes an action to be executed
*/
int main(int argc, char *argv[])
{

    char *action;

    // validate the amount of given uer parameters
    if (argc == 1 || argc > 2)
    {
        printf("%s\n", ERROR_PARAMETERS_NUM);
        return EXIT_FAILURE;
    }

    // the action argument
    action = argv[1];

    // case the argument was "best"
    if (strcmp(action, COMMAND_BEST) == 0)
    {
        actionBest();

        return EXIT_SUCCESS;
    }

    // case the argument was "merge"
    if (strcmp(action, COMMAND_MERGE) == 0)
    {
        actionMerge();

        return EXIT_SUCCESS;
    }

    // case the argument was "quick"
    if (strcmp(action, COMMAND_QUICK) == 0)
    {
        actionQuick();
        return EXIT_SUCCESS;
    }

    // case invalid argument was given
    {
        printf("%s\n", ERROR_PARAMETERS_WRONG_TYPE);
        return EXIT_FAILURE;
    }
}

/**
 * @brief performs the "best" action as was  described in the exercise documentation
 * */
void actionBest()
{
    getUserEntriesInput();
    printBestStudent();
}

/**
 * @brief performs the "merge" action as was  described in the exercise documentation
 * */
void actionMerge()
{
    getUserEntriesInput();
    mergeSort(0, totalStudentsEntries - 1);
    printStudentsInfo();
}

/**
 * @brief performs the "quick" action as was  described in the exercise documentation
 * */
void actionQuick()
{
    getUserEntriesInput();
    quickSort(0, totalStudentsEntries - 1);
    printStudentsInfo();

}

/**
 * @brief add a user to the studentEntries array from an input line provided by the user
 * */
void addStudentEntry(char *line)
{

    char id[ID_FIELD_LENGTH + 1];
    char name[DEFAULT_FIELD_LENGTH + 1];
    int grade;
    int age;
    char country[DEFAULT_FIELD_LENGTH + 1];
    char city[DEFAULT_FIELD_LENGTH + 1];

    // get the data of the new student
    sscanf(line, "%s\t%[^\t]\t%d\t%d\t%s\t%s\t", id, name, &grade, &age, country, city);

    // and to the student entries
    strcpy(studentEntries[totalStudentsEntries].id, id);
    strcpy(studentEntries[totalStudentsEntries].name, name);
    studentEntries[totalStudentsEntries].grade = grade;
    studentEntries[totalStudentsEntries].age = age;
    strcpy(studentEntries[totalStudentsEntries].country, country);
    strcpy(studentEntries[totalStudentsEntries].city, city);

    // increase the total count
    totalStudentsEntries++;
}

/**
 * @brief requests input student from the user.
 * The explicit way and format of the student input is described in the exercise documentation
 *
 * */
void getUserEntriesInput()
{

    char line[LINE_MAX_LENGTH];
    int lineCount = 0;


    // the main loop
    while (lineCount < MAX_INPUT_ROWS)
    {

        // request to the user to enter data
        printf("%s", ENTER_INFO_MESSAGE);


        // takes input line from the terminal
        fgets(line, LINE_MAX_LENGTH, stdin);

        // check whenever the user entered the exit code
        if (strcmp(line, EXIT_COMMAND) == 0)
        {
            break;
        }

        // check the validity of the input
        if (validateStudentInputLine(line, lineCount))
        {

            // add the user to the students entries array
            addStudentEntry(line);

        }

    }

    lineCount++;
}


/**
 * @brief compare 2 students quality as described in the exercise documentation
 * @param firstStudent
 * @param secondsStudent
 * */
int compareStudentsQuality(Student firstStudent, Student secondsStudent)
{

    int firstStudentGrade = firstStudent.grade;
    int firstStudentAge = firstStudent.age;
    int secondStudentGrade = secondsStudent.grade;
    int secondStudentAge = secondsStudent.age;

    float firstStudentQuality = ((float) firstStudentGrade / firstStudentAge);
    float secondStudentQuality = ((float) secondStudentGrade / secondStudentAge);


    // checks whenever the values are the same
    if (fabs(firstStudentQuality - secondStudentQuality) < EPSILON)
    {
        return 0;
    }

    // check whenever the second value is larger
    if (firstStudentQuality < secondStudentQuality)
    {
        return -1;
    }

    return 1;


}

/**
 * @brief prints to the console a given student info
 * @param student
 * */
void printStudentInfo(Student student)
{
    printf("%s\t%s\t%d\t%d\t%s\t%s\t\n", student.id, student.name, student.grade, student.age,
           student.country,
           student.city);
}

/**
 * @brief prints info of all the students in a given array
 *
 * */
void printStudentsInfo()
{
    int i;

    for (i = 0; i < totalStudentsEntries; i++)
    {
        printStudentInfo(studentEntries[i]);
    }
}

/**
 * @brief goes over an array of students,find the one with the best quailty and prints to the
 * console
 * */
void printBestStudent()
{

    int i;
    int bestIndex = 0;

    //   check whenever there are any students
    if (totalStudentsEntries == 0)
    {
        return;
    }

    //  find the best student
    for (i = 0; i < totalStudentsEntries; i++)
    {

        if (compareStudentsQuality(studentEntries[bestIndex], studentEntries[i]) < 0)
        {
            bestIndex = i;
        }
    }

    // print  the best student info
    printf("%s", MESSAGE_FOUND_BEST);
    printStudentInfo(studentEntries[bestIndex]);
}

/**
 * Swap  given Students
 * @param firstStudent
 * @param secondStudent
 * */
void swapStudents(Student *firstStudent, Student *secondStudent)
{
    Student temp = *firstStudent;
    *firstStudent = *secondStudent;
    *secondStudent = temp;
}

/**
 * @brief partition as part of the quicksort algorithm
 * @param left The lower bound
 * @param right The upper bound
 * */
int partition(int left, int right)
{
    Student pivot = studentEntries[right]; // pivot
    int i = (left - 1); //  Index of smaller element
    for (int j = left; j <= right - 1; j++)
    {

        // If current element is smaller that the  pivot
        if (strcmp(studentEntries[j].name, pivot.name) < 0)
        {

            i++; // increment index of smaller element
            swapStudents(&studentEntries[i], &studentEntries[j]);
        }
    }


    swapStudents(&studentEntries[i + 1], &studentEntries[right]);

    return (i + 1);
}


/**
 * @brief Sorts an array of students by their name.Uses the quickSort algorithm
 * where the pivot is always the last element in the array
 * @param left The lower bound for the sort
 * @param right The upper bound for the sort
 * */
void quickSort(int left, int right)
{
    if (left < right)
    {

        // find the pivot and partition the array
        int pivot = partition(left, right);


        quickSort(left, pivot - 1);
        quickSort(pivot + 1, right);

    }
}

/**
 * Merges sub elements in a student array
 * */
void merge(int l, int m, int r)
{
    int i, j, k;
    int n1 = m - l + 1;
    int n2 = r - m;


    // Copy the date to the first helper array
    for (i = 0; i < n1; i++)
    {

        leftMergeSortArray[i] = studentEntries[l + i];
    }

    // Copy the date to the second helper array
    for (j = 0; j < n2; j++)
    {
        rightMergeSortArray[j] = studentEntries[m + 1 + j];
    }

    /* Merge the temp arrays back into arr[l..r]*/
    i = 0; // Initial index of first subarray
    j = 0; // Initial index of second subarray
    k = l; // Initial index of merged subarray

    while (i < n1 && j < n2)
    {
        if (leftMergeSortArray[i].grade <= rightMergeSortArray[j].grade)
        {
            studentEntries[k] = leftMergeSortArray[i];
            i++;
        }
        else
        {
            studentEntries[k] = rightMergeSortArray[j];
            j++;
        }
        k++;
    }

    /* Copy the remaining elements of L[], if there
       are any */
    while (i < n1)
    {
        studentEntries[k] = leftMergeSortArray[i];
        i++;
        k++;
    }

    /* Copy the remaining elements of R[], if there
       are any */
    while (j < n2)
    {
        studentEntries[k] = rightMergeSortArray[j];
        j++;
        k++;
    }
}

/**
 * @brief Sorts an array of students by their grade.Uses the mergeSort algorithm
 * @param studentEntries
 * @param left The lower bound of the merge
 * @param right The upper bound of the merge
 * */
void mergeSort(int left, int right)
{

    if (left < right)
    {
        int middle = left + (right - left) / 2;

        // Sort the  first and second values
        mergeSort(left, middle);
        mergeSort(middle + 1, right);

        // merges the created sub-arrays
        merge(left, middle, right);
    }
}


/**
 * @brief Counts the tabs of a given string
 * @param line the string to check
 * */
int getTabCount(char *line)
{
    char ch;
    int i = 0;
    int wordCount = 0;

    while ((ch = line[i]))
    {
        if (ch == '\t')
        {
            wordCount++;
        }
        i++;
    }
    return wordCount;
}

/**
 * @brief checks whenever a provided string contains only numerical chars
 * @param input the string to check
 * */
bool isNumber(char *input)
{
    int i = 0;

    while (input[i])
    {
        // checks whenver the number is negative
        if (i == 0 && (!isdigit(input[i])) && ('-' != (input[i])))
        {
            return false;
        }

        // checks whenever a digit is given
        if (i != 0 && !isdigit(input[i]))
        {
            return false;
        }
        i++;
    }
    return true;
}

/**
 * @brief converts a string into a number
 * @param str a given string
 * @return  The numerical value of the string
 * */
int getNumberFromString(char *str)
{
    int number;
    sscanf(str, "%d", &number);
    return number;
}

/**
 * @brief checks whenever a provided string contains only alphabetic characters or '-'
 * @param input the string to check
 * */
bool isValidWord(char *input)
{
    int i = 0;
    while (input[i])
    {
        if ((!(((input[i] >= 'a') && (input[i] <= 'z')) && input[i] != '-' && input[i] != ' ')) &&
            (!((input[i] >= 'A' && input[i] <= 'Z')) && input[i] != '-' && input[i] != ' '))
        {
            return 0;
        }
        ++i;
    }
    return 1;
}

/**
 * @brief check whenever a provided input   is a valid repesentaion of student data,per the
 * requierments described in the excersice description
 * @param line line to validate
 * @param line the current line
 * */
bool validateStudentInputLine(char *line, int lineCount)
{
    char foundID[DEFAULT_FIELD_LENGTH];
    char foundName[DEFAULT_FIELD_LENGTH];
    char foundGrade[DEFAULT_FIELD_LENGTH];
    char foundAge[DEFAULT_FIELD_LENGTH];
    char foundCountry[DEFAULT_FIELD_LENGTH];
    char foundCity[DEFAULT_FIELD_LENGTH];

    // check the amount of provided fields
    if ((getTabCount(line)) != EXPECTED_TOTAL_FIELDS)
    {
        printf("%sin line %d\n", ERROR_FIELD_NUM, lineCount);
        return false;
    }

    // proccess the input
    sscanf(line, "%s\t%[^\t]\t%s\t%s\t%s\t%s\t", foundID, foundName, foundGrade, foundAge,
           foundCountry, foundCity);

    // validate the found ID field :

    if (!isNumber(foundID))
    {
        printf("%sin line %d\n", ID_TYPE_ERROR_MSG, lineCount);

        return false;
    }

    if (strcmp(foundID, MIN_ID) <= 0)
    {
        printf("%sin line %d\n", ID_VALUE_ERROR_MSG, lineCount);

        return false;
    }

    if (strlen(foundID) != 10)
    {
        printf("%sin line %d\n", ID_TYPE_ERROR_MSG, lineCount);

        return false;
    }

    // validate the found Name field :

    if (!isValidWord(foundName))
    {
        printf("%sin line %d\n", NAME_TYPE_ERROR_MSG, lineCount);

        return false;
    }

    // validate the found Grade field :

    if (!isNumber(foundGrade))
    {
        printf("%sin line %d\n", GRADE_TYPE_ERROR_MSG, lineCount);

        return false;
    }

    if (!isInRange(getNumberFromString(foundGrade), MIN_GRADE, MAX_GRADE))
    {

        printf("%sin line %d\n", GRADE_VALUE_ERROR_MSG, lineCount);

        return false;
    }

    // validate the found Age field :

    if (!isNumber(foundAge))
    {
        printf("%sin line %d\n", AGE_TYPE_ERROR_MSG, lineCount);

        return false;
    }

    if (!isInRange(getNumberFromString(foundAge), MIN_AGE, MAX_AGE))
    {
        printf("%sin line %d\n", AGE_VALUE_ERROR_MSG, lineCount);

        return false;
    }
    // validate the found Country field :

    if (!isValidWord(foundCountry))
    {
        printf("%sin line %d\n", COUNTRY_TYPE_ERROR_MSG, lineCount);

        return false;
    }
    // validate the found City field :

    if (!isValidWord(foundCity))
    {
        printf("%sin line %d\n", CITY_TYPE_ERROR_MSG, lineCount);

        return false;
    }

    return true;

}

/**
 * @brief checks whenever a given number is in an inclusive range
 * @param number The number to check
 * @param   lowerLimit The lower limit
 * @param   upperLimit The Upper limit
 * */
bool isInRange(int number, int lowerLimit, int upperLimit)
{
    return (lowerLimit <= number && number <= upperLimit);
}
