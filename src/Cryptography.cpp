#include <ExerciseCollection/Cryptography.hpp>
#include <cryptopp/default.h>
#include <cryptopp/files.h>
#include <cryptopp/hex.h>
#include <cryptopp/osrng.h>	 // for AutoSeededRandomPool
#include <cryptopp/rsa.h>

namespace Cryptography
{
#pragma region encryption
	auto caesar_encrypt(std::string_view text, const int shift) -> std::string
	{
		std::string result;
		result.reserve(text.length());
		for(const auto cha : text)
		{
			result += (isalpha(cha) && isupper(cha)) ? 'A' + (cha - 'A' + shift) % 26 : cha;
		}
		return result;
	}

	auto caesar_decrypt(std::string_view text, const int shift) -> std::string
	{
		std::string result;
		result.reserve(text.length());
		for(const auto cha : text)
		{
			result += (isalpha(cha) && isupper(cha)) ? 'A' + (26 + cha - 'A' - shift) % 26 : cha;
		}
		return result;
	}

	auto get_vigenere_table() -> std::string_view
	{
		static std::string table;
		if(table.empty())
		{
			table.reserve(26 * 26);
			for(int i = 0; i < 26; ++i)
			{
				table += caesar_encrypt("ABCDEFGHIJKLMNOPQRSTUVWXYZ", i);
			}
		}
		return table;
	}

	auto vigenere_encrypt(std::string_view text, std::string_view key) -> std::string
	{
		std::string result;
		result.reserve(text.length());
		for(size_t i = 0; i < text.length(); ++i)
		{
			auto row = key[i % key.length()] - 'A';
			auto col = text[i] - 'A';
			result += get_vigenere_table()[row * 26 + col];
		}
		return result;
	}

	auto vigenere_decrypt(std::string_view text, std::string_view key) -> std::string
	{
		std::string result;
		result.reserve(text.length());
		for(size_t i = 0; i < text.length(); ++i)
		{
			auto row = key[i % key.length()] - 'A';
			for(size_t col = 0; col < 26; col++)
			{
				if(get_vigenere_table()[row * 26 + col] == text[i])
				{
					result += 'A' + col;
					break;
				}
			}
		}
		return result;
	}
#pragma endregion

#pragma region file encryption
	auto get_hash(std::string_view password) -> std::string
	{
		CryptoPP::SHA512 sha;
		CryptoPP::byte	 digest[CryptoPP::SHA512::DIGESTSIZE];
		sha.CalculateDigest(
			digest, reinterpret_cast<const CryptoPP::byte*>(password.data()), password.length());
		CryptoPP::HexEncoder encoder;
		std::string			 result;
		encoder.Attach(new CryptoPP::StringSink(result));
		encoder.Put(digest, sizeof(digest));
		encoder.MessageEnd();
		return result;
	}

	template<class Hash>
	std::string compute_file_hash(const fs::path& filepath)
	{
		std::string digest;
		Hash		hash;

		CryptoPP::FileSource source{
			filepath.c_str(),
			true,
			new CryptoPP::HashFilter(hash,
									 new CryptoPP::HexEncoder(new CryptoPP::StringSink(digest)))};
		return digest;
	}

	auto get_file_hash_SHA1(const fs::path& filepath) -> std::string
	{
		return compute_file_hash<CryptoPP::SHA1>(filepath);
	}

	auto get_file_hash_SHA256(const fs::path& filepath) -> std::string
	{
		return compute_file_hash<CryptoPP::SHA256>(filepath);
	}

	void
	encrypt_file(const fs::path& sourcefile, const fs::path& destfile, std::string_view password)
	{
		using namespace CryptoPP;

		FileSource source(sourcefile.c_str(),
						  true,
						  new DefaultEncryptorWithMAC((byte*)password.data(),
													  password.size(),
													  new FileSink(destfile.c_str())));
	}

	void encrypt_file(const fs::path& filepath, std::string_view password)
	{
		try
		{
			auto temppath = fs::temp_directory_path() / filepath.filename();
			encrypt_file(filepath, temppath, password);
			fs::remove(filepath);
			fs::rename(temppath, filepath);
		}
		catch(const std::exception& e)
		{
			std::cerr << e.what() << std::endl;
		}
	}

	void
	decrypt_file(const fs::path& sourcefile, const fs::path& destfile, std::string_view password)
	{
		using namespace CryptoPP;

		FileSource source(sourcefile.c_str(),
						  true,
						  new DefaultDecryptorWithMAC((byte*)password.data(),
													  password.size(),
													  new FileSink(destfile.c_str())));
	}

	void decrypt_file(const fs::path& filepath, std::string_view password)
	{
		try
		{
			auto temppath = fs::temp_directory_path() / filepath.filename();
			decrypt_file(filepath, temppath, password);
			fs::remove(filepath);
			fs::rename(temppath, filepath);
		}
		catch(const std::exception& e)
		{
			std::cerr << e.what() << std::endl;
		}
	}
#pragma endregion

#pragma region file signing
	CryptoPP::AutoSeededRandomPool rng;

	void encode(const fs::path& filepath, CryptoPP::BufferedTransformation const& bt)
	{
		CryptoPP::FileSink file(filepath.c_str());
		bt.CopyTo(file);
		file.MessageEnd();
	}

	void encode_private_key(const fs::path& filepath, CryptoPP::RSA::PrivateKey const& key)
	{
		CryptoPP::ByteQueue queue;
		key.DEREncodePrivateKey(queue);
		encode(filepath, queue);
	}

	void encode_public_key(const fs::path& filepath, CryptoPP::RSA::PublicKey const& key)
	{
		CryptoPP::ByteQueue queue;
		key.DEREncodePublicKey(queue);
		encode(filepath, queue);
	}

	void decode(const fs::path& filepath, CryptoPP::BufferedTransformation& bt)
	{
		CryptoPP::FileSource file(filepath.c_str(), true);
		file.TransferTo(bt);
		bt.MessageEnd();
	}

	void decode_private_key(fs::path const& filepath, CryptoPP::RSA::PrivateKey& key)
	{
		CryptoPP::ByteQueue queue;
		decode(filepath, queue);
		key.BERDecodePrivateKey(queue, false, queue.MaxRetrievable());
	}

	void decode_public_key(fs::path const& filepath, CryptoPP::RSA::PublicKey& key)
	{
		CryptoPP::ByteQueue queue;
		decode(filepath, queue);
		key.BERDecodePublicKey(queue, false, queue.MaxRetrievable());
	}

	void generate_keys(fs::path const& privateKeyPath, fs::path const& publicKeyPath)
	{
		using namespace CryptoPP;
		try
		{
			RSA::PrivateKey rsaPrivate;
			rsaPrivate.GenerateRandomWithKeySize(rng, 3072);
			RSA::PublicKey rsaPublic(rsaPrivate);
			encode_private_key(privateKeyPath, rsaPrivate);
			encode_public_key(publicKeyPath, rsaPublic);
		}
		catch(CryptoPP::Exception const& e)
		{
			std::cerr << e.what() << std::endl;
		}
	}

	void rsa_sign_file(const fs::path& filepath,
					   const fs::path& privateKeyPath,
					   const fs::path& signaturePath)
	{
		using namespace CryptoPP;

		RSA::PrivateKey privateKey;
		decode_private_key(privateKeyPath, privateKey);
		RSASSA_PKCS1v15_SHA_Signer signer(privateKey);
		FileSource				   fileSource(filepath.c_str(),
							  true,
							  new SignerFilter(rng, signer, new FileSink(signaturePath.c_str())));
	}

	auto rsa_verify_file(const fs::path& filepath,
						 const fs::path& publicKeyPath,
						 const fs::path& signaturePath) -> bool
	{
		using namespace CryptoPP;

		RSA::PublicKey publicKey;
		decode_public_key(publicKeyPath.c_str(), publicKey);
		RSASSA_PKCS1v15_SHA_Verifier verifier(publicKey);
		FileSource					 signatureFile(signaturePath.c_str(), true);
		if(signatureFile.MaxRetrievable() != verifier.SignatureLength())
		{
			return false;
		}

		SecByteBlock signature(verifier.SignatureLength());
		signatureFile.Get(signature, signature.size());
		auto* verifierFilter = new SignatureVerificationFilter(verifier);
		verifierFilter->Put(signature, verifier.SignatureLength());
		FileSource fileSource(filepath.c_str(), true, verifierFilter);

		return verifierFilter->GetLastResult();
	}
#pragma endregion
}  // namespace Cryptography