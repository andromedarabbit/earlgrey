                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                  #pragma once
#include "Attachment.h"
#include "MailAddress.h"
#include "MailMessage.h"

#include <msclr\marshal.h>
// <msclr\marshal.h>, <msclr\marshal_windows.h>, <msclr\marshal_cppstd.h>, or <msclr\marshal_atl.h>
#include <msclr\marshal_cppstd.h>

namespace msclr
{
	namespace interop
	{
		template<>
		inline System::Net::Mail::Attachment^ marshal_as(const Earlgrey::Mail::Attachment& from_obj)
		{
			System::String^ fileName = marshal_as<System::String^>(from_obj.FileName().c_str());
			System::String^ mediaType = marshal_as<System::String^>(from_obj.MediaType().c_str());

			return gcnew System::Net::Mail::Attachment(fileName, mediaType);
		}

		template<>
		inline System::Net::Mail::Attachment^ marshal_as(const Earlgrey::Mail::MailMessage::AttachmentPtr& from_obj)
		{
			Earlgrey::Mail::Attachment * nativeObj = from_obj.get();
			return marshal_as<System::Net::Mail::Attachment^>(*nativeObj);
		}

		template<>
		inline System::Net::Mail::MailAddress^ marshal_as(const Earlgrey::Mail::MailAddress& from_obj)
		{
			System::String^ address = marshal_as<System::String^>(from_obj.Address().c_str());
			System::String^ displayName = marshal_as<System::String^>(from_obj.DisplayName().c_str());

			return gcnew System::Net::Mail::MailAddress(address, displayName, System::Text::Encoding::UTF8);
		}

		template<>
		inline System::Net::Mail::MailAddress^ marshal_as(const Earlgrey::Mail::MailMessage::MailAddressPtr& from_obj)
		{
			Earlgrey::Mail::MailAddress * nativeObj = from_obj.get();
			return marshal_as<System::Net::Mail::MailAddress^>(*nativeObj);
		}

		template<>
		inline System::Net::Mail::MailMessage^ marshal_as(const Earlgrey::Mail::MailMessage& from_obj)
		{
			System::String^ subject = marshal_as<System::String^>(from_obj.Subject().c_str());
			System::String^ body = marshal_as<System::String^>(from_obj.Body().c_str());

			System::Net::Mail::MailMessage^ to_obj = gcnew System::Net::Mail::MailMessage();
			to_obj->Subject = subject;
			to_obj->Body = body;

			Earlgrey::Mail::MailMessage::AttachmentCollection::const_iterator it_a = from_obj.Attachments().begin();
			for( ; it_a != from_obj.Attachments().end(); it_a++)
			{
				System::Net::Mail::Attachment^ managedAttachment = marshal_as<System::Net::Mail::Attachment^>(*it_a);
				to_obj->Attachments->Add( managedAttachment );
			}

			Earlgrey::Mail::MailMessage::MailAddressCollection::const_iterator it_ma = from_obj.To().begin();
			for( ; it_ma != from_obj.To().end(); it_ma++)
			{
				System::Net::Mail::MailAddress^ managedAddress = marshal_as<System::Net::Mail::MailAddress^>(*it_ma);
				to_obj->To->Add( managedAddress );
			}

			System::Net::Mail::MailAddress^ fromAddress = marshal_as<System::Net::Mail::MailAddress^>(from_obj.From());
			to_obj->From = fromAddress;

			return to_obj;
		}

	} // !interop

} // !msclr
