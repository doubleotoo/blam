# Blam                                                                                                                                                                                                  
                                                                                                                                                                                                        
Blam is a text snippet manager. It manages your text snippets. From the command line.                                                                                                                   
It's totally inspired by [Boom](http://github.com/holman/boom). However, as you may know, Boom is written in Ruby.                                                                                      
Being far too self-righteous to use something written in Ruby, however awesome it may be, I decided to port it to Python.                                                                               
It was mostly an exercise in my hacking ability - I wanted to see if I could write something useful without access to the internet.                                                                     
So when the time came for a 7 hour car ride from Philly to Hatteras Island, I grabbed what was left of my summer reading and my trusty old MBP.                                                         
As a result of my limited (zero) internet access when the majority of the code was written, it's very hacky. Most things are probably done entirely the wrong way. But it works.                        
Anyway, that's my spiel. Have fun. Use protection.                                                                                                                                                      
                                                                                                                                                                                                        
# Install                                                                                                                                                                                               
To install, download the tarball however you wish (No, I dont have it all in a fancy package. My bad.) and stick blam in /usr/bin/, or wherever you feel like putting it.                               
                                                                                                                                                                                                        
For autocompletion support, assuming you use bash, put the contents of completion.bash in your .bashrc or .bash_profile files, depending which system you're on.                                        
                                                                                                                                                                                                        
Also, make sure you have termcolor installed. If you dont,                                                                                                                                              
                                                                                                                                                                                                        
      pip install termcolor                                                                                                                                                                             
                                                                                                                                                                                                        
Blam will create a config file at ~/.blam. Inside is the JSON data that holds all of your blam stuff. Feel free to edit this manually.                                                                  
Copying to clipboard with blam only works if                                                                                                                                                            
                                                                                                                                                                                                        
        printf something | xsel -c                                                                                                                                                                      
copies "something" to your clipboard on Linux, or if you're on OS X,                                                                                                                                    
                                                                                                                                                                                                        
       printf something | pbcopy                                                                                                                                                                        
You can always tailor the copy command to your setup, provided you're not opposed to changing a few lines in the script.
There is semi-intentionally no Windows support.

# Usage
Usage is nearly identical to that of [Boom](http://github.com/holman/boom). You can type
     
     blam help

for more details, or you can refer to [Boom's usage wiki](https://github.com/holman/boom/wiki/Commands), of course changing all of the boom's to blam's.
[Email me](mailto:hunterlang@comcast.net) with any additional questions regarding features or usage.