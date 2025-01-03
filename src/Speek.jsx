import React, { useRef } from 'react';
import Speech from 'react-speech';

const MyComponent = () => {
//   const { speak, stop } = useSpeech();

const ref=useRef();
  const handleSpeak = () => {
    console.log(ref);
    ref.current.innerText='Speak'
  };

  const handleStop = () => {
    // stop();
  };

  return (
    <div>
         <Speech onClick={handleSpeak} ref={ref} text="Welcome to react speech" displayText="Speak" textAsButton={true} />
   
    </div>
  );
};

export default MyComponent;