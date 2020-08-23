#include <Audio.h>

class TEENSY_AUDIO_STREAM_WRAPPER : public AudioStream
{
    audio_block_t*                  m_input_queue_array[1];
    
protected:
    
    // these are the only functions that require bespoke Teensy code
    bool                            process_audio_in( int channel )
    {        
        audio_block_t* read_block        = receiveReadOnly();
        
        if( read_block != nullptr )
        {
            process_audio_in_impl( channel, read_block->data, AUDIO_BLOCK_SAMPLES );
            release( read_block );
            
            return true;
        }
        
        return false;
    }
    
    bool                            process_audio_out( int channel )
    {      
        audio_block_t* write_block = allocate();
        
        if( write_block != nullptr )
        {
            process_audio_out_impl( channel, write_block->data, AUDIO_BLOCK_SAMPLES );
            
            transmit( write_block, channel );
            
            release( write_block );
            
            return true;
        }
        
        return false;
    }
    
    // add audio processing code in these 2 functions
    virtual void                    process_audio_in_impl( int channel, const int16_t* sample_data, int num_samples ) = 0;
    virtual void                    process_audio_out_impl( int channel, int16_t* sample_data, int num_samples ) = 0;
    
public:
    
    TEENSY_AUDIO_STREAM_WRAPPER() :
        AudioStream( 1, m_input_queue_array ),
        m_input_queue_array()
    {
        
    }
    
    virtual ~TEENSY_AUDIO_STREAM_WRAPPER()      {;}
    
    virtual int                     num_input_channels() const = 0;
    virtual int                     num_output_channels() const = 0;
};
